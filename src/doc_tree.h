#ifndef DOC_TREE_H
#define DOC_TREE_H

#include "ltree.h"

/* forward declaration of doc_elt */
struct doc_elt;
typedef struct doc_elt doc_elt;

typedef ltree_node doc_node;
typedef doc_node doc_tree;
typedef ltree_list doc_list;

static inline doc_elt *
doc_node_get_elt (doc_node *node)
{
  return (doc_elt *) ltree_node_get_data (node);
}

static inline void
doc_node_set_elt (doc_node *node, doc_elt *elt)
{
  ltree_node_set_data ((ltree_node *)node, (void *)elt);
}

static inline doc_node *
doc_node_get_parent (doc_node *node)
{
  return (doc_node *)(ltree_node_get_parent ((ltree_node *)node));
}

static inline void
doc_node_set_parent (doc_node *node, doc_node *parent)
{
  ltree_node_set_parent ((ltree_node *)node, (ltree_node *)parent);
  return;
}

static inline doc_list
doc_node_get_children (doc_node *node)
{
  return (doc_list)(ltree_node_get_children ((ltree_node *)node));
}

static inline void
doc_node_set_children (doc_node *node, doc_list *children)
{
  ltree_node_set_children ((ltree_node *)node, (ltree_list)children);
  return;
}

#endif
