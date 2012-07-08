/**
 * @file merge_tree.h 
 *
 * A wrapper to ltree, used to represent a merge tree.  A merge tree
 * is an ltree that stores only merge_delta as data.
 */

#ifndef MERGE_TREE_H
#define MERGE_TREE_H

#include "ltree.h"

struct merge_delta;
typedef struct merge_delta merge_delta;

typedef ltree      merge_tree;
typedef ltree_node merge_node;
typedef ltree_list merge_list;

static inline merge_delta *
merge_node_get_delta (merge_node *node)
{
  return (merge_delta *)(ltree_node_get_data ((ltree_node *)node));
}

static inline void
merge_node_set_delta (merge_node *node, merge_delta * delta)
{
  ltree_node_set_data ((ltree_node *)node, (void *)delta);
}

static inline merge_node *
merge_node_get_parent (merge_node *node)
{
  return (merge_node *)(ltree_node_get_parent ((ltree_node *)node));
}

static inline void
merge_node_set_parent (merge_node *node, merge_node *parent)
{
  ltree_node_set_parent ((ltree_node *)node, (ltree_node *)parent);
  return;
}

static inline merge_list
merge_node_get_children (merge_node *node)
{
  return (merge_list)ltree_node_get_children ((ltree_node *)node);
}

static inline void
merge_node_set_children (merge_node *node, merge_list children)
{
  ltree_node_set_children ((ltree_node *)node, (ltree_list)children);
  return;
}

#endif
