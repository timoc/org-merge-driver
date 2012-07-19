/**
 * @file merge_map.h
 */

#ifndef MERGE_MAP_H
#define MERGE_MAP_H

#include <stdlib.h>
#include <stddef.h>

#include "merge_change.h"

struct merge_delta;
typedef struct merge_delta merge_delta;

/**
 * A common identity between 3 merge_delta objects.
 */
typedef struct merge_map
{
  merge_delta_change ancestor;
  merge_delta_change local;
  merge_delta_change remote;
} merge_map;

/**
 * Indicates an input document (ancestor, local, or source). Used to
 * indicate the document from which a doc_* or merge_* object
 * originates.
 */
typedef enum doc_src
  {
    src_ancestor = (offsetof (struct merge_map, ancestor)),
    src_local    = (offsetof (struct merge_map, local)),
    src_remote   = (offsetof (struct merge_map, remote))
  } doc_src;

/**
 * An element's key, indexed by
 */
typedef int map_key;
/**
 * An element
 */
typedef int map_set;
static const int NO_MAP_SET = -1;

static inline merge_delta_change *
merge_map_get_delta_change (merge_map *map, doc_src src)
{
  char *c = (char *)map;
  return (merge_delta_change *)(c + src);
}

static inline void
merge_map_set_delta (merge_map *map, doc_src src, merge_delta *delta)
{
  char *c = (char * )map;
  merge_map_get_delta_change(map, src)->delta = delta;
  return;
}

static inline merge_delta *
merge_map_get_delta (merge_map *map, doc_src src)
{
  char *c = (char *)map;
  return ((merge_delta_change *)(c + src))->delta;
}

static inline void
merge_map_set_change (merge_map *map, doc_src src, merge_change change)
{
  char *c = (char *)map;
  ((merge_delta_change *)(c + src))->change = change;
  return;
}

static inline merge_change
merge_map_get_change (merge_map *map, doc_src src)
{
  char *c = (char *)map;
  return ((merge_delta_change *)(c + src))->change;
}

inline static merge_map *
merge_map_create_empty ()
{
  merge_map *map =  malloc (sizeof (merge_map));

  merge_map_set_change (map, src_ancestor, unmapped);
  merge_map_set_delta (map, src_ancestor, NULL);

  merge_map_set_change (map, src_local, unmapped);
  merge_map_set_delta (map, src_local, NULL);

  merge_map_set_change (map, src_remote, unmapped);
  merge_map_set_delta (map, src_remote, NULL);

  return map;
}

inline static void
merge_map_free (merge_map *map)
{
  free (map);
  return;
}

#endif
