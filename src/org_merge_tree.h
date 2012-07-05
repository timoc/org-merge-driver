/**
 * @file org_merge_tree
 */
#ifndef ORG_MERGE_TREE_H
#define ORG_MERGE_TREE_H

struct org_element;

/**
 * @brief A node in the merge tree.
 */
struct org_mnode;

/**
 * @brief An entire merge tree.
 */
typedef struct org_mnode org_mtree;

enum org_mnode_state  {no_update, insert, delete, move};
enum org_mnode_source {ancestor_file, local_file, remote_file, both_files};

struct org_mnode
{
  enum org_mnode_state  state;
  enum org_mnode_source source;
  struct org_mapping   *mapping;
};
/**
 * @brief Create a tree representing a merged tree (with conflicts).
 */
org_mtree *
create_org_mtree (struct org_element *ancestor, 
		  struct org_element *local,
		  struct org_element *remote);

/* Utility Functions */
void
apply_org_mtree (org_mtree *mtree, void (*f)(struct org_mnode *));

/* Gets and Sets */
static inline struct org_mapping *
get_org_mnode_mapping (struct org_mnode *mnode)
{
  return mnode->mapping;
}

static inline void
set_org_mnode_mapping (struct org_mnode *mnode, struct org_mapping *mapping)
{
  mnode->mapping = mapping;
}

static inline enum org_mnode_state
get_org_mnode_state (struct org_mnode *mnode)
{
  return mnode->state;
}

static inline void
set_org_mnode_state (struct org_mnode *mnode, enum org_mnode_state state)
{
  mnode->state = state;
}

static inline enum org_mnode_source
get_org_mnode_source (struct org_mnode *mnode)
{
  return mnode->source;
}

static inline void
set_org_mnode_source (struct org_mnode *mnode, enum org_mnode_source source)
{
  mnode->source = source;
}

#endif
