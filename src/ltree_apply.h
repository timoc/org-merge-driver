/**
 * @file merge_tree_apply.h 
 * helps write functions which can apply on each node in prefix or
 * postfix order (or both)
 */

/*
 * can define: 
 *   FUNC_NAME  the name of the function
 *   CONTEXT_GLOBAL  a global context
 *   CONTEXT_DOWN    a context which only is passed downwards
 *   CONTEXT_LOC     a context for local data
 *   START_FUNC      statements for side effects, called in prefik order
 *   END_FUNC        statements for side effects, called in postfix order
 */

#include "ltree.h"

#ifndef FUNC_NAME
#define FUNC_NAME merge_tree_apply
#endif

void
FUNC_NAME (ltree *tree
#ifdef CONTEXT_DOWN
	   ,CONTEXT_DOWN *ct_down
#endif
#ifdef CONTEXT_GLOBAL
	   ,CONTEXT_GLOBAL *ct_glob
#endif
	   )
{
#ifdef CONTEXT_LOCAL
  CONTEXT_LOCAL ct_loc;
#endif

  /*
   * Copy the passed context.
   * The currently root node will use and modify the copy.
   * The modified copy will be passed to the children.
   * The original will be unchanged.
   * This is to create "inherited" contexts.
   */
#ifdef CONTEXT_DOWN
  CONTEXT_DOWN new_ct_down;
  memcpy(&new_ct_down, ct_down, sizeof (CONTEXT_DOWN));
#endif

#ifdef START_FUNC
  START_FUNC
#endif

    ltree_list list = ltree_node_get_children (tree);
  if (list != NULL)
    {
      size_t list_size = gl_list_size (list);
      size_t i = 0;
      for (i = 0; i < list_size; i++)
	{
	  FUNC_NAME (ltree_node_get_child_at (tree, i)
#ifdef CONTEXT_DOWN
		     ,&new_ct_down
#endif
#ifdef CONTEXT_GLOBAL
		     ,ct_glob
#endif
		     );
	}
    }
#ifdef END_FUNC
  END_FUNC
#endif
      
    return;
}
