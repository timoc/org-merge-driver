/**
 * @file merge_delta.h
 */

#ifndef MERGE_DELTA_H
#define MERGE_DELTA_H

#include <stdlib.h>
#include "merge_map.h"
#include "merge_change.h"

struct doc_elt;
typedef struct doc_elt doc_elt;


typedef struct merge_delta
{
  doc_src src;
  doc_elt *elt;
  merge_map *map;
  int child_update;
} merge_delta;

static inline merge_delta *
merge_delta_create_empty ()
{
  merge_delta *md = malloc (sizeof (merge_delta));
  md->elt = NULL;
  md->map = NULL;
  md->src = 0;
  return md;
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

static inline void
merge_delta_set_elt (merge_delta *delta, doc_elt *elt)
{
  delta->elt = elt;
  return;
}

static inline doc_src
merge_delta_get_doc_src (merge_delta *delta)
{
  return delta->src;
}

static inline void
merge_delta_set_doc_src (merge_delta *delta, doc_src src)
{
  delta->src = src;
  return;
}

static inline void
merge_delta_set_type (merge_delta *delta, doc_src src)
{
  delta->src = src;
  return;
}

static inline void
mark_child_update (merge_delta * delta)
{
  /**
   * @todo Make this do something better
   */
  delta->child_update = true;
  return;
}
#endif
