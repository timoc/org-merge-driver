/**
 * @file doc_merge.h
 */
#ifndef DOC_MERGE_H
#define DOC_MERGE_H

#include "merge_tree.h"

/**
 * @brief Create a tree representing a merged tree (with conflicts).
 */
merge_tree * create_merge_tree (doc_tree *ancestor, doc_tree *local, doc_tree *remote);

#endif
