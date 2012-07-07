/**
 * @file doc_tree.h
 */

#ifndef DOC_TREE_H
#define DOC_TREE_H

#include <stdlib.h>
#include "gl_list.h"

typedef struct doc_tree_node doc_tree_node;
typedef doc_tree_node doc_tree;

typedef struct doc_tree_node
{
  void *elt;
  struct doc_tree_node *parent;
  gl_list_t children;
} doc_tree_node;

static inline doc_tree_node *
doc_tree_node_create_empty ()
{
  return malloc (sizeof (doc_tree_node));
}

static inline void
doc_tree_node_free (doc_tree_node *node)
{
  free (node);
  /**
   * @todo doc_tree_node must free children & elt recursively when
   * freed. Perhaps should also free parent if not already null...
   */
}

static inline void *
doc_tree_node_get_elt (doc_tree_node *node)
{
  return node->elt;
}
  
static inline void
doc_tree_node_set_elt (doc_tree_node *node, void *elt)
{
  node->elt = elt;
  return;
}

static inline doc_tree_node *
doc_tree_node_get_parent (doc_tree_node *node)
{
  return node->parent;
}

static inline void
doc_tree_node_set_parent (doc_tree_node *node, doc_tree_node *parent)
{
  node->parent = parent;
  return;
}

static inline gl_list_t
doc_tree_node_get_children (doc_tree_node *node)
{
  return node->children;
}

static inline void
doc_tree_node_set_children (doc_tree_node *node, gl_list_t children)
{
  node->children = children;
}

#endif
