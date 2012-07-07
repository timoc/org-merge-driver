#ifndef DOC_TREE_H
#define DOC_TREE_H

#include "ltree.h"
#include "doc_elt.h"

typedef ltree      doc_tree;
typedef ltree_node doc_node;
typedef ltree_list doc_list;

static inline doc_elt *
doc_node_get_elt (doc_node * node)
{
  return (doc_elt *)(ltree_node_get_data (node));
}

static inline void
doc_node_set_elt (doc_node *node, doc_elt *elt)
{
}

#endif
