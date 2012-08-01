/**
 * @file doc_ref.c
 */

#include <stdlib.h>
#include "debug.h"
#include "config.h"
#include "gl_list.h"

/* for difseq */
#include <limits.h>
#include <stdbool.h>
#include "minmax.h"

#include "doc_elt.h"
#include "doc_ref.h"

bool
doc_reflist_isupdated (gl_list_t reflist)
{
  gl_list_iterator_t i;
  i = gl_list_iterator (reflist);
  doc_ref *ref = NULL;
  bool updated =  false;
  while (!updated
	 && gl_list_iterator_next (&i, (const void **) &ref, NULL))
    {
      /*
      if (!doc_ref_contains (ref, ANC_SRC))
	{
	  updated = true;
	  break;
	}
      */
      updated = doc_elt_isupdated (ref);
    }
  debug_msg (DOC, 4, "reflist updated = %d\n", updated);
  gl_list_iterator_free (&i);
  return updated;
}

void
doc_reflist_print (gl_list_t reflist, void *context, doc_stream *out)
{
  /**
   * @todo make the print ctxt pass around correctly
   */
  debug_msg (DOC, 5, "Printing element list\n");
  gl_list_iterator_t i;
  i = gl_list_iterator (reflist);
  doc_ref *ref;
  doc_elt *elt;

  while (gl_list_iterator_next (&i, (const void **) &ref, NULL))
    {
      debug_msg (DOC, 4, "Printing element\n");
      elt = doc_ref_get_elt (ref);
      doc_elt_print (ref, context, out);
    }
  debug_msg (DOC, 5, "Finished printing element\n");
  gl_list_iterator_free (&i);
  return;
}

struct context;

typedef enum mapped_state
  {
    state_mapped   = 0,
    state_unmapped = 1
  } mapped_state;

#define OFFSET int
#undef USE_HEURISTIC
#undef ELEMENT
#undef EQUAL

static void note_delete (struct context *ctxt, OFFSET xoff);
static void note_insert (struct context *ctxt, OFFSET yoff);
static int  is_related  (struct context *ctxt, OFFSET xoff, OFFSET yoff);

#define EXTRA_CONTEXT_FIELDS			\
  mapped_state *a_state;			\
  mapped_state *d_state;			\
  gl_list_t     ancestor;			\
  gl_list_t     descendant;

#define NOTE_DELETE(ctxt, xoff) note_delete (ctxt, xoff)
#define NOTE_INSERT(ctxt, yoff) note_insert (ctxt, yoff)
#define XVECREF_YVECREF_EQUAL(ctxt, xoff, yoff) is_related (ctxt, xoff, yoff)

#include "diffseq.h"
#define OFFSET int

static void
note_delete (struct context *ctxt, OFFSET xoff)
{
  debug_msg (MERGE, 3, "deleting %d\n", xoff);
  ctxt->a_state[xoff] = state_unmapped;
  return;
}

static void
note_insert (struct context *ctxt, OFFSET yoff)
{
  debug_msg (MERGE, 3, "inserting %d\n", yoff);
  ctxt->d_state[yoff] = state_unmapped;
  return;
}

static int
is_related (struct context *ctxt, OFFSET xoff, OFFSET yoff)
{
  doc_ref *a_ref = (doc_ref * )gl_list_get_at (ctxt->ancestor, xoff);
  doc_ref *d_ref = (doc_ref *)gl_list_get_at (ctxt->descendant, yoff);

  int result = doc_elt_isrelated (a_ref, d_ref, NULL);
  debug_msg (MERGE, 4, "comparing (%d, %d)=%d\n", xoff, yoff, result);
  return result;
}

void
doc_reflist_merge (gl_list_t ancestor, gl_list_t descendant, merge_ctxt *merge_ctxt)
{
  /* First, create a mapped_state for every element that will be mapped.
   * Compare the two strigs marking mapped and unmapped nodes.  Then,
   * merge the two lists, combining mapped elements ino a sigle element.
   */

  size_t a_size = gl_list_size (ancestor);
  size_t d_size = gl_list_size (descendant);

  /* Create a mapped state for every element. Must initialize to 0. */
  mapped_state *a_state = calloc (a_size, sizeof (mapped_state));
  mapped_state *d_state = calloc (d_size, sizeof (mapped_state));
  debug_msg (MERGE, 3, "Start merge, sizes %d and %d\n", a_size, d_size);


  /* Send the elements through compareseq.  compareseq will leave
   * add/remove notes in the mapped_states.
   */

  /* prepare the compareseq context */
  struct context ctxt;

  /* Add the caller data */
  ctxt.d_state    = d_state;
  ctxt.a_state    = a_state;
  ctxt.ancestor   = ancestor;
  ctxt.descendant = descendant;

  /* Allocate data for the algorithm to use*/
  size_t diags = a_size + d_size + 3;
  void *mem    = malloc (diags * (2 * sizeof (OFFSET)));
  ctxt.fdiag   = mem;
  ctxt.bdiag   = ctxt.fdiag + diags;
  ctxt.fdiag  += d_size + 1;
  ctxt.bdiag   = ctxt.fdiag + diags;

  /* run a diffseq on the elements */
  compareseq (0,                     /* children index lower bound   */
	      a_size,                /* children index upper bound   */
	      0,                     /* children index lower bound   */
	      d_size,                /* children index upper bound   */
	      1,                     /* find optimal solution        */
	      &ctxt);                /* difseq context created above */

  debug_msg (MERGE, 3, "Finish compareseq\n");
  free (mem);

  /*
   * Create the mappings and update the merge_tree.
   *
   * go through the list in the end and
   * - when there is a 'matched' nodes
   *   - delete the matched elements delta, mark a 'change' in the mapping,
   *   - point the mapping to the element
   * - when there is an insert:
   *   - add the dnode at that point
   *  - when there is a delete:
   *    - mark it in the mapping
   */

  int a_index = 0; /* # of checked elt's in *tree list */
  int d_index = 0;
  int inserted = 0; /* number of nodes inserted into the ancestor
		       tree. Used to make sure the index stays
		       correct */
  int next = 0; /* between two inserted nodes, which one is next */

  while ((a_index != a_size)
	 || (d_index != d_size))
    {
#if 0
      /* If they are unmapped, pick the order to put them in the list */
      if ((d_index != d_size) &&
	  (d_state[d_index] == state_unmapped) &&
	  (a_index != a_size) &&
	  (a_state[a_index-inserted] == state_unmapped))
	{
	  /**
	   * @todo replace this with a function that lets the parent decide the order
	   */
	  if (0)
	    {
	      next = 1;
	    }
	  else
	    {
	      next = 2;
	    }
	}
      else
#endif
	if ((d_index != d_size)
	    && (d_state[d_index] == state_unmapped))
	  {
	    next = 1;
	  }
	else if ((a_index != a_size)
		 && (a_state[a_index-inserted] == state_unmapped))
	  {
	    next = 2;
	  }
      else
	{
	  /* both are mapped */
	  next = 0;
	}

      if (next == 1)
	{
	  /* Insert an unmatched node */
	  debug_msg (MERGE, 3, "Inserting node\n");
	  doc_ref * d_ref = (doc_ref *) gl_list_get_at (descendant, d_index);
	  gl_list_nx_add_at (ancestor, a_index, (void *) d_ref);

	  //mark_insert_children ();
	  doc_elt_note_insert (d_ref, merge_ctxt);

	  a_index++;
	  a_size++;
	  inserted++;
	  d_index++;
	}
      else if (next == 2)
	{
	  /* Delete an unmatched node */
	  debug_msg (MERGE, 3, "Removing node\n");
	  doc_ref * a_ref = (doc_ref *) gl_list_get_at (ancestor, d_index);

	  // mark_remove_children (m_child, src);
	  doc_elt_note_delete (a_ref, merge_ctxt);
	  a_index++;
	}
      else  if ((a_index != a_size) && (d_index != d_size))
	{
	  /* Match and merge matched nodes */
	  debug_msg (MERGE, 3, "Matching node\n");
	  doc_ref * a_ref = (doc_ref *) gl_list_get_at (ancestor, a_index);
	  doc_ref * d_ref = (doc_ref *) gl_list_get_at (descendant, d_index);

	  assert ((a_state[a_index-inserted] == state_mapped) &&
		  (d_state[d_index] == state_mapped));

	  /* set the document source of the doc_ref to the combined sources */
	  doc_ref_add_src (a_ref, doc_ref_get_src (d_ref));

	  /* get the element to merge the content and children */
	  doc_elt_merge (a_ref, d_ref, merge_ctxt);

	  /* make the doc_refs point to the same element */
	  doc_ref_set_elt (d_ref, doc_ref_get_elt (a_ref) );

	  a_index++;
	  d_index++;
	}
    }

  free (a_state);
  free (d_state);
  return;
}
