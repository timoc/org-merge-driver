/**
 * @file merge_delta.h
 */

#ifndef MERGE_DELTA_H
#define MERGE_DELTA_H

#include <stdlib.h>
#include "merge_map.h"
#include "merge_change.h"

struct merge_map;
typedef struct merge_map merge_map;

struct doc_elt;
typedef struct doc_elt doc_elt;

typedef struct merge_delta
{
  doc_src src;
  doc_elt *elt;
  merge_change type;
  merge_map *map;
} merge_delta;

static inline merge_delta *
merge_delta_create_empty ()
{
  return malloc (sizeof (merge_delta));
}

static inline void
merge_delta_free (merge_delta *delta)
{
  free (delta);
}

static inline merge_map *
merge_delta_get_map (merge_delta *delta)
{
  return delta->map;
}

static inline void
merge_delta_set_map (merge_delta * delta, merge_map *map)
{
  delta->map = map;
  return;
}

static inline doc_elt *
merge_delta_get_elt (merge_delta *delta)
{
  return delta->elt;
}

#endif
