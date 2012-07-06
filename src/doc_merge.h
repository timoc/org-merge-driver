/**
 * @file doc_merge.h
 */
#ifndef DOC_MERGE_H
#define DOC_MERGE_H

struct doc_tree_node;
typedef struct doc_tree_node doc_tree_node;
typedef doc_tree_node doc_tree;



/**
 * @brief An entire merge tree.
 */
typedef doc_tree merge_tree;

/**
 * @brief A node in the merge tree.
 */
typedef doc_tree_node merge_tree_node;

/**
 * @brief Create a tree representing a merged tree (with conflicts).
 */
merge_tree *
merge_doc_trees (doc_tree *ancestor, doc_tree *local, doc_tree *remote);

void
merge_tree_apply (merge_tree *tree, void (*lambda)(merge_tree_node *node));

/*
struct doc_tree_node;
typedef struct doc_tree_node doc_tree_node;

typedef doc_tree_node doc_tree;

struct doc_tree_map;
typedef struct doc_tree_map doc_tree_map;

struct merge_tree_node;
typedef struct merge_tree_node merge_tree_node;

merge_tree_node * merge_tree_node_create_empty ();

void merge_tree_node_free (merge_tree_node *node);

doc_tree_delta * merge_tree_node_get_delta (merge_tree_node *node);

void merge_tree_node_set_delta (merge_tree_node *node, doc_tree_delta *delta);

doc_tree_map * merge_tree_node_get_map (merge_tree_node *node);

void merge_tree_node_set_map (merge_tree_node *node, doc_tree_map *map);
*/

#endif
