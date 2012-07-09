/**
 * @file ltree.h
 */

#ifndef LTREE_H
#define LTREE_H

#include <stdlib.h>
#include "config.h"
#include "gl_array_list.h"
#include "gl_list.h"

struct ltree_node;
typedef struct ltree_node ltree_node;
typedef ltree_node ltree;
typedef gl_list_t ltree_list;

typedef struct ltree_node
{
  void *data;
  ltree_node *parent;
  ltree_list children;
} ltree_node;

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
ltree_node_get_parent (ltree_node *node)
{
  return node->parent;
}

static inline void
ltree_node_set_parent (ltree_node *node, ltree_node *parent)
{
  node->parent = parent;
  return;
}

static inline ltree_node *
ltree_node_create_empty ()
{
  ltree_node *l = malloc (sizeof (ltree_node));
  ltree_node_set_children (l, gl_list_nx_create_empty (GL_ARRAY_LIST, NULL, 
						       NULL, NULL, true));
  return l;
}

static inline ltree_node*
ltree_node_get_child_at (ltree_node *node, int index)
{
  gl_list_t list = ltree_node_get_children (node);
  return (ltree_node *)gl_list_get_at (list, index);
}

static inline void
ltree_node_add_child_last (ltree_node *node, void*e)
{
  gl_list_t list = ltree_node_get_children (node);
  gl_list_nx_add_last (list, (void*) e);
  return;
}

static inline ltree_node *
ltree_node_create ()
{
  return  malloc (sizeof (ltree_node));
}

static inline void
ltree_node_free (ltree_node *node)
{
  free (node);
}

#endif
