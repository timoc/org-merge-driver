/**
 * @file ltree.h
 */

#ifndef LTREE_H
#define LTREE_H

#include <stdlib.h>
#include "gl_list.h"

struct ltree_node;
typedef struct ltree_node ltree_node;
typedef ltree ltree_node;
typedef gl_list_t ltree_list;

typedef struct doc_tree_node
{
  void *data;
  doc_tree_node *parent;
  ltree_list *children;
} doc_tree_node;

static inline doc_tree_node *
ltree_node_create_empty ()
{
  return malloc (sizeof (ltree_node));
}

static inline void
ltree_node_free (ltree_node *node)
{
  free (node);
}

static inline void *
ltree_node_get_data (ltree_node *node)
{
  return node->data;
}
  
static inline void
ltree_node_set_data (ltree_node *node, void *data)
{
  node->data = data;
  return;
}

static inline ltree_node *
ltree_node_set_parent (ltree_node *node)
{
  return node->parent;
}

static inline void
ltree_node_set_parent (ltree_node *node, ltree_node *parent)
{
  node->parent = parent;
  return;
}

static inline ltree_list
ltree_node_get_children (ltree_node *node)
{
  return node->children;
}

static inline void
ltree_node_set_children (ltree_node *node, ltree_list children)
{
  node->children = children;
}

#endif
