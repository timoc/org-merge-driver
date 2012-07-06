/**
 * @file doc_tree_map.h
 */

#ifndef DOC_TREE_MAP_H
#define DOC_TREE_MAP_H

#include <stdlib.h>

struct doc_tree_delta;
typedef struct doc_tree_delta doc_tree_delta;

struct doc_tree_node;
typedef struct doc_tree_node doc_tree_node;

typedef struct doc_tree_map
{
  doc_tree_node *ancestor;
  doc_tree_node *local;
  doc_tree_node *remote;
  doc_tree_delta *local_delta;
  doc_tree_delta *remote_delta;
} doc_tree_map;

typedef enum doc_tree_src
  {
    ancestor_src = offsetof (doc_tree_map, ancestor),
    local_src    = offsetof (doc_tree_map, local),
    remote_src   = offsetof (doc_tree_map, remote)
  } doc_tree_src;

static inline doc_tree_node *
doc_tree_map_get_node (doc_tree_map *map, doc_tree_src src)
{
  char *c = (char *)map;
  return (doc_tree_node *) (c + src);
}

static inline doc_tree_delta *
doc_tree_map_get_delta (doc_tree_map *map, doc_tree_src src)
{
  if (src == local_src)
    return map->local_delta;
  else if (src == remote_src)
    return map->remote_delta;
  // fail if anything else
  assert(0);
  return NULL;
}
inline static doc_tree_map *
doc_tree_map_create_empty ()
{
  return malloc (sizeof (doc_tree_map));
}

inline static void
doc_tree_map_free (doc_tree_map *map)
{
  free (map);
  return;
}

inline static doc_tree_node *
doc_tree_map_get_ancestor (doc_tree_map *map)
{
  return map->ancestor;
}

inline static void
doc_tree_map_set_ancestor (doc_tree_map *map, doc_tree_node *ancestor)
{
  map->ancestor = ancestor;
  return;
}

inline static doc_tree_node *
doc_tree_map_get_local (doc_tree_map *map)
{
  return map->local;
}

inline static void
doc_tree_map_set_local (doc_tree_map *map, doc_tree_node *local)
{
  map->local = local;
  return;
}

inline static doc_tree_node *
doc_tree_map_get_remote (doc_tree_map *map)
{
  return map->remote;
}

inline static void
doc_tree_map_set_remote (doc_tree_map *map, doc_tree_node *remote)
{
  map->remote = remote;
  return;
}

inline static doc_tree_delta *
doc_tree_delta_get_local_delta (doc_tree_map *map)
{
  return map->local_delta;
}

inline static void
doc_tree_map_set_local_delta (doc_tree_map *map, doc_tree_delta *local)
{
  map->local_delta = local;
  return;
}

inline static doc_tree_delta *
doc_tree_delta_get_remote_delta (doc_tree_map *map)
{
  return map->remote_delta;
}

inline static void
doc_tree_map_set_remote_delta (doc_tree_map *map, doc_tree_delta *remote)
{
  map->remote_delta = remote;
  return;
}

#endif
