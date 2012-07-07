/**
 * @file merge_map.h
 */

#ifndef MERGE_MAP_H
#define MERGE_MAP_H

#include <stdlib.h>

struct merge_delta;
typedef struct merge_delta merge_delta;

struct doc_node;
typedef struct doc_node doc_node;

typedef struct merge_map
{
  doc_node *ancestor;
  doc_node *local;
  doc_node *remote;
  merge_delta *local_delta;
  merge_delta *remote_delta;
} merge_map;

typedef enum merge_src
  {
    ancestor_src = offsetof (merge_map, ancestor),
    local_src    = offsetof (merge_map, local),
    remote_src   = offsetof (merge_map, remote)
  } merge_src;

static inline doc_node *
merge_map_get_node (merge_map *map, merge_src src)
{
  char *c = (char *)map;
  return (doc_node *) (c + src);
}

static inline merge_delta *
merge_map_get_delta (merge_map *map, merge_src src)
{
  if (src == local_src)
    return map->local_delta;
  else if (src == remote_src)
    return map->remote_delta;
  // fail if anything else
  assert(0);
  return NULL;
}
inline static merge_map *
merge_map_create_empty ()
{
  return malloc (sizeof (merge_map));
}

inline static void
merge_map_free (merge_map *map)
{
  free (map);
  return;
}

inline static doc_node *
merge_map_get_ancestor (merge_map *map)
{
  return map->ancestor;
}

inline static void
merge_map_set_ancestor (merge_map *map, doc_node *ancestor)
{
  map->ancestor = ancestor;
  return;
}

inline static doc_node *
merge_map_get_local (merge_map *map)
{
  return map->local;
}

inline static void
merge_map_set_local (merge_map *map, doc_node *local)
{
  map->local = local;
  return;
}

inline static doc_node *
merge_map_get_remote (merge_map *map)
{
  return map->remote;
}

inline static void
merge_map_set_remote (merge_map *map, doc_node *remote)
{
  map->remote = remote;
  return;
}

inline static merge_delta *
merge_delta_get_local_delta (merge_map *map)
{
  return map->local_delta;
}

inline static void
merge_map_set_local_delta (merge_map *map, merge_delta *local)
{
  map->local_delta = local;
  return;
}

inline static merge_delta *
merge_map_get_remote_delta (merge_map *map)
{
  return map->remote_delta;
}

inline static void
merge_map_set_remote_delta (merge_map *map, merge_delta *remote)
{
  map->remote_delta = remote;
  return;
}

#endif
