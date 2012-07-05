#include <stdbool.h>
#include "org_element.h"
#include "org_mapping.h"
#include "org_merge_tree.h"

/**
 * @brief Add all sub-elements of an inserted node to the merge tree
 */
static void insert_node (struct org_mnode *root, enum org_mapping_offset other);

/**
 * @brief Remove all sub-elements of removed node to the merge tree
 */
static void remove_node (struct org_mnode *root, enum org_mapping_offset ancestor);

static struct org_mnode*
create_org_mnode()
{
  return malloc (sizeof(struct org_mnode));
}

/**
 * @brief create the mapping of the sub elements of two matched nodes
 */
static void
map_child_org_elements (struct org_mnode *base, 
			enum org_mapping_offset ancestor, 
			enum org_mapping_offset other);

org_mtree *
create_org_mtree (struct org_element *ancestor,
		  struct org_element *local, 
		  struct org_element *remote)
{
  /* Create a root node to store the root of the document */
  struct org_mnode *root = create_org_mnode();
  set_org_mnode_source  (root, both_files);

  /* Point the root mnode to a mapping of all three roots */
  struct org_mapping *mapping = create_org_mapping();  
  set_org_mnode_mapping (root, mapping);

  /* Map the root of each document to each other; a set of mapped
   * elements all share a single dnode in the delta tree */
  set_org_mapping_ancestor (mapping, ancestor);
  set_org_mapping_local    (mapping, local);
  set_org_mapping_remote   (mapping, remote);

  /* call the mapping algorithm twice, so it gets all changes from
   * both files */
  map_child_org_elements (root, org_mapping_ancestor, org_mapping_local);
  map_child_org_elements (root, org_mapping_ancestor, org_mapping_remote);

  /* Return the delta_tree, which can then be merge printed */
  return (org_mtree*) root;
}

static void
map_child_org_elements (struct org_mnode *base, 
			enum org_mapping_offset ancestor, 
			enum org_mapping_offset other)
{
  /* Find the nodes which were inserted and deleted */
  //diff_list(base[base_offset].sub_elements, base[other_offset]);

  /* assume that the previous function told each sub_element if it
     were inserted or removed.  For each sub element we must now create a
     dnode for it in the merge if it was added, deleted, or unchanged,
     and add it to a sub list */
  /* we cannot assume that the list of dnodes is empty, since this
     function willbe called on two pairs of files was added */
  /* Create an iterator for each list.  For the base list, if a node
     was deleted, we add all elements to the delta tree as deleted
     elements.  For the other list, if an element was inserted, we
     call insert for all of its sub elements. */
  /* since we can't assume that the sub dnodes don't exist, we have to
     check to make sure that the base element does not already have a
     mapping */

  /*
  bool quit = false;
  org_element *e1 = base;
  while (!quit)
    {
      e1 from base[base_offset].sub_elements;
      e2 from base[other_offset].sub_elements;
      if (e2.status == INSERT)
	{
	  /* an element from the other list has been added
	  struct dnode *d = create_dnode();
	  d = e2;
	  add_sub_dnode (d);
	}
    }
*/
}

static void
insert_node (struct org_mnode *root, enum org_mapping_offset other)
{
  return;
}

static void
remove_node (struct org_mnode *root, enum org_mapping_offset ancestor)
{
  return;
}
