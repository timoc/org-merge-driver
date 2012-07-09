#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "gl_list.h"

#include "merge_delta.h"
#include "doc_stream.h"
#include "doc_elt.h"
#include "doc_elt_ops.h"
#include "merge_print_ctxt.h"
#include "merge_print.h"

void
merge_print (merge_tree *tree, const merge_print_ctxt *old_ctxt, doc_stream *out)
{
  /*
   * Obtain the delta stored in the root node.
   * Mergeprint the delta's map's elements.
   */
  //printf("\nmerge_print: start.. ");
  merge_delta *delta = merge_node_get_delta (tree);
  if (delta != NULL)
    {
      //printf("non null delta.. ");
      /*
       * Copy the passed context.
       * The currently root node will use and modify the copy.
       * The modified copy will be passed to the children.
       * The original will be unchanged.
       * This is to create "inherited" contexts.
       */
      merge_print_ctxt new_ctxt;
      memcpy(&new_ctxt, old_ctxt, sizeof (merge_print_ctxt));

      /**
       * @todo check merge_delta's references to map, elt, elt_ops
       */
      doc_elt *elt = merge_delta_get_elt(delta);
      if (elt != NULL) {
	//printf("printing:\n");
	//doc_elt_print (elt, NULL, out);
	doc_elt_merge_print (elt, delta, &new_ctxt, out);
      }
      else {
	//printf("null element, skip.. ");
      }

      /*
      doc_elt_ops *ops = doc_elt_get_ops (elt);
      doc_elt_ops_get_merge_print(ops)(delta, &new_ctxt, out);
      */
      /*
       * Mergeprint the current node's children.
       */
      merge_list list = merge_node_get_children (tree);  
      if (list != NULL)
	{
	  size_t list_size = gl_list_size (list);
	  size_t i = 0;
	  //printf(" %d children: ", list_size);
	  for (i = 0; i < list_size; i++)
	    {
	      //printf("\nchild %d ", i);
	      merge_print (merge_node_get_child_at (tree, i), &new_ctxt, out);
	      //merge_print ((merge_tree *)gl_list_get_at (list, i), &new_ctxt, out);
	    }
	}
      //printf("\nDONE");
    }

  return;
}

/*
run tests/t1-0.org tests/t1-1.org tests/t1-2.org
*/
