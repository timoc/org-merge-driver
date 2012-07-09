#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

#include "doc_tree.h"
#include "merge_map.h"
#include "merge_delta.h"
#include "doc_elt.h"
#include "merge_tree.h"
#include "merge_change.h"

/* for difseq */
#include "config.h"
#include <limits.h>
#include <stdbool.h>
#include "minmax.h"

#include "doc_merge.h"

/**
 * @brief Merge a doc_tree into a merge tree, using rules created in the doc_elts
 */
static void
doc_tree_merge (merge_tree *mtree, doc_tree *dtree, doc_src src);

/**
 * @brief will mark all of the children nodes in the merge tree as removed for the src
 */
static void
mark_remove_children  (merge_tree *mtree, doc_src src);

/**
 * @brief Insert all the children of dtree into an empty mtree
 */
static void
insert_children  (merge_tree *mtree, doc_tree *dtree, doc_src src);

merge_tree *
create_merge_tree (doc_tree *ancestor, doc_tree *local, doc_tree *remote)
{
  /* create a root node for the merge tree */
  merge_tree *merge_root = ltree_node_create_empty ();
  merge_delta *root_delta = merge_delta_create_empty ();
  merge_node_set_delta (merge_root, root_delta);

  /**
   * @TODO: set the delta to point to this map
   * @TODO: set the map to point back to the delta for all three elements
   */
  //merge_map *root_map = merge_map_create_empty ();

  /* merge all three trees onto tho root of the merge_tree */
  doc_tree_merge (merge_root, ancestor, src_ancestor);
  doc_tree_merge (merge_root, local, src_local);
  doc_tree_merge (merge_root, remote, src_remote);

  return ((merge_tree *) merge_root);
}

struct context;

#define OFFSET int
#undef USE_HEURISTIC
#undef ELEMENT
#undef EQUAL

static void note_delete (struct context *ctxt, OFFSET xoff);
static void note_insert (struct context *ctxt, OFFSET yoff);
static int compare (struct context *ctxt, OFFSET xoff, OFFSET yoff);

#define EXTRA_CONTEXT_FIELDS			\
  gl_list_t m_delta;				\
  merge_delta * d_delta;

#define NOTE_DELETE(ctxt, xoff) note_delete (ctxt, xoff)
#define NOTE_INSERT(ctxt, yoff) note_insert (ctxt, yoff)
#define XVECREF_YVECREF_EQUAL(ctxt, xoff, yoff) compare (ctxt, xoff, yoff)

#include "diffseq.h"
#define OFFSET int

static void
doc_tree_merge (merge_tree *mtree_root, doc_tree *dtree_root, doc_src src)
{
  /*
   * How this function works:
   * 1. create a new merge_delta for every doc_node in the doc_tree
   *    - merge_deltas are created as an array.
   * 2. assume that the merge_tree deltas are already set 'matched'
   * 3. run cmpseq on the two lists
   * 3.1 every time an element is inserted or deleted, it will be noted in its delta
   * 4. use the 'edits' stored in the deltas to create a mappings
   * 4.1. reset the value of all delta mappings to matched simultaneusly
   * 4.2 store in the mapping the result of the merge in the mapping
   * 4.3 store inserted element deltas in the list at the point where they were inserted
   */

  /*
   * Establish basic facts about each list 
   */
  ltree_list mtree_children = ltree_node_get_children (mtree_root);
  size_t mtree_child_count = gl_list_size (mtree_children);

  ltree_list dtree_children = ltree_node_get_children (dtree_root);
  size_t dtree_child_count = gl_list_size (dtree_children);

  /*
   * Create a delta for every child of the doc_tree.
   */

  /* allocate the deltas in an array */
  merge_delta *dtree_deltas = calloc (dtree_child_count, sizeof (merge_delta));

  /*  Initialize the deltas */
  size_t i;
  for (i = 0; i < dtree_child_count; i++)
    {
      dtree_deltas[i].src  = src;
      dtree_deltas[i].elt  = ltree_node_get_data (((ltree_node *) gl_list_get_at 
						   (dtree_children, i)));
      dtree_deltas[i].type = unchanged;
    }

  /* 
   * Send the delta lists through compareseq.
   * 
   * compareseq will leave add/remove notes in the deltas. Notes are
   * left in the deltas as a convenience.
   */

  /* prepare the compareseq context */
  struct context ctxt;

  /* Add the caller data */
  ctxt.d_delta = dtree_deltas;
  ctxt.m_delta = mtree_children;

  /* Allocate data for the algorithm */
  size_t diags = mtree_child_count + dtree_child_count + 3;
  void *mem    = malloc (diags * (2 * sizeof (OFFSET)));
  ctxt.fdiag   = mem;
  ctxt.bdiag   = ctxt.fdiag + diags;
  ctxt.fdiag  += dtree_child_count + 1;
  ctxt.bdiag   = ctxt.fdiag + diags;

  /* run a diffseq on the elements */
  compareseq (0,                     /* mtree_children index lower bound */
	      mtree_child_count,     /* mtree_children index upper bound */
	      0,                     /* dtree_children index lower bound */
	      dtree_child_count,     /* dtree_children index upper bound */
	      1,                     /* find optimal solution            */
	      &ctxt);                /* difseq context created above     */

  /*
   * Create the mappings and update the merge_tree.
   */

  /* go through the list in the end and 
   * - when there is a 'matched' nodes
   *   - delete the matched elements delta, mark a 'change' in the mapping, 
   *   - point the mapping to the element
   * - when there is an insert:
   *   - add the dnode at that point
   *  - when there is a delete:
   *    - mark it in the mapping
   */

  int mtree_index = 0; /* # of checked elt's in *tree list */
  int dtree_index = 0;
  merge_map * new_map;

  while ((mtree_index != mtree_child_count) 
	 || (dtree_index != dtree_child_count))
    {
      while (dtree_index != dtree_child_count
	     && dtree_deltas[dtree_index].type == change_insert)
	{

	  merge_delta *d_delta = &(dtree_deltas[dtree_index]);
	  d_delta->type = unchanged;

	  /* a dtree's delta must be inserted into the mtree_children list here
	   *
	   * 1. create a mapping 
	   * 2. Add the delta to the mapping 
	   * 3. Mark the mapping as 'inserted' by the document being merged 
	   * 4. Insert the delta into the merge tree 
	   * 5. Increment dtree_index, mtree_child_count
	   *    - increment mtree_index also, to move past newly inserted node.
	   * 6. add the node's children to the list as 'inserted'
	   *    - the unmatched node's children are also unmatched
	   */
	  /* create the mapping */
	  new_map = merge_map_create_empty ();
	  merge_map_set_delta (new_map, src, &dtree_deltas[dtree_index]);
	  merge_map_set_change (new_map, src, mapped);
	  merge_delta_set_map (&dtree_deltas[dtree_index], new_map);

	  /* add the delta to the merge_tree */
	  merge_node *new_node = ltree_node_create_empty ();
	  ltree_node_set_data (new_node, &dtree_deltas[dtree_index]);
	  gl_list_nx_add_at (mtree_children, mtree_index, new_node);

	  /* Add all the children as inserted */
	  insert_children (new_node, (merge_tree *) gl_list_get_at (dtree_children, dtree_index), src);

	  /* Advance the mtree_index and child_count */
	  mtree_index++;
	  mtree_child_count++;
	  dtree_index++;

	}
      while (mtree_index != mtree_child_count
	     && ((merge_delta *) ltree_node_get_data 
		 ((merge_node *) gl_list_get_at (mtree_children, mtree_index))
		 )->type == change_remove)
	{
	  merge_node *m_child = (merge_node *)gl_list_get_at (mtree_children, mtree_index);
	  merge_delta *m_delta = ltree_node_get_data (m_child);
	  merge_delta *d_delta = &(dtree_deltas[dtree_index]);
	  merge_map *map =  merge_delta_get_map (m_delta);

	  /* 1. Mark the mtree node's delta as unmatched by the new document
	   * 2. Mark all of thi children as having not existed in this file
	   * 3. Increment the mtree_index and move on.
	   */
	  m_delta->type = unchanged;
	  merge_map_set_delta (map, src, NULL);
	  merge_map_set_change (map, src, unmapped);
	  mtree_index++;
	  mark_remove_children (m_child, src);
	}

      if ((mtree_index != mtree_child_count) && (dtree_index != dtree_child_count))
	{
	  merge_node *m_child = (merge_node *)gl_list_get_at (mtree_children, mtree_index);
	  merge_delta *m_delta = ltree_node_get_data (m_child);
	  merge_delta *d_delta = &(dtree_deltas[dtree_index]);

	  assert ((d_delta->type == unchanged) && (m_delta->type == unchanged));

	  /* 1. Add the dtree_delta to the mtree_delta's mapping, since they must match
	   * 2. Recurse matching function, using newly matched nodes as root nodes.
	   */
	  merge_map *map =  merge_delta_get_map (m_delta);
	  merge_map_set_delta (map, src, d_delta);
	  merge_map_set_change (map, src, mapped);
	  doc_tree_merge (m_child, (merge_tree *)gl_list_get_at (dtree_children, dtree_index), src);
	  mtree_index++;
	  dtree_index++;
	}
    }
  free (mem);
  return;
}

static void
mark_remove_children  (merge_tree *mtree, doc_src src)
{
 /**
  * @todo: fix the quick cheese */
  // recursively mark all children nodes as removed
  doc_tree_merge (mtree, ltree_node_create_empty(), src);
}

static void
insert_children  (merge_tree *mtree, doc_tree *dtree, doc_src src)
{
  /**
   * @todo: fix the quick cheese */
  doc_tree_merge (mtree, dtree, src);
}

static
void note_delete (struct context *ctxt, OFFSET xoff)
{
  merge_delta *m_d =  (ltree_node_get_data ((merge_node *)gl_list_get_at (ctxt->m_delta, xoff)));
  m_d->type = change_remove;
  return;
}

static
void note_insert (struct context *ctxt, OFFSET yoff)
{
  ctxt->d_delta[yoff].type = change_insert;
  return;
}

static
int compare (struct context *ctxt, OFFSET xoff, OFFSET yoff)
{
  gl_list_t list = ctxt->m_delta;
  merge_node *m_n = (merge_node *)gl_list_get_at (list, xoff);
  merge_delta *m_d = merge_node_get_delta (m_n);
  merge_delta d_d = ctxt->d_delta[yoff];

  return doc_elt_compare (d_d.elt, d_d.src, m_d->elt, m_d->src, NULL);
}
