/**
 * @file doc_tree_delta.h
 */

#ifndef DOC_TREE_DELTA_H
#define DOC_TREE_DELTA_H

#include <stdlib.h>

struct doc_tree_map;
typedef struct doc_tree_map doc_tree_map;

typedef enum doc_tree_delta_type 
  {
    doc_tree_node_no_change,
    doc_tree_node_update,
    doc_tree_node_delete,
    doc_tree_node_add,
    doc_tree_node_move
  } doc_tree_delta_type;

typedef enum doc_tree_delta_src
  {
    doc_tree_delta_local_src,
    doc_tree_delta_remote_src,
    doc_tree_delta_ancestor_src
  } doc_tree_delta_src;

typedef struct doc_tree_delta
{
  doc_tree_delta_type type;
  doc_tree_delta_src src;
  doc_tree_map *map;
} doc_tree_delta;

static inline doc_tree_delta *
doc_tree_delta_create_empty ()
{
  return malloc (sizeof (doc_tree_delta));
}

static inline void
doc_tree_delta_free (doc_tree_delta *delta)
{
  free (delta);
}

static inline doc_tree_delta_type
doc_tree_delta_get_type (doc_tree_delta *delta)
{
  return delta->type;
}

static inline void
doc_tree_delta_set_type (doc_tree_delta *delta, doc_tree_delta_type type)
{
  delta->type = type;
  return;
}

static inline doc_tree_delta_src
doc_tree_delta_get_src (doc_tree_delta *delta)
{
  return delta->src;
}

static inline void
doc_tree_delta_set_src (doc_tree_delta *delta, doc_tree_delta_src src)
{
  delta->src = src;
  return;
}

static inline doc_tree_delta *
doc_tree_delta_create (doc_tree_delta_type type, doc_tree_delta_src src)
{
  doc_tree_delta *d = doc_tree_delta_create_empty();
  doc_tree_delta_set_type (d, type);
  doc_tree_delta_set_src (d, src);
  return d;
}

static inline doc_tree_map *
doc_tree_delta_get_map (doc_tree_delta *delta)
{
  return delta->map;
}

static inline void
doc_tree_delta_set_map (doc_tree_delta *delta, doc_tree_map *map)
{
  delta->map = map;
  return;
}

#endif
