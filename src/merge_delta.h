/**
 * @file merge_delta.h
 */

#ifndef MERGE_DELTA_H
#define MERGE_DELTA_H

#include <stdlib.h>

struct merge_map;
typedef struct merge_map merge_map;

typedef enum merge_delta_type 
  {
    merge_no_change,
    merge_update,
    merge_delete,
    merge_add,
    merge_move
  } merge_delta_type;

typedef enum merge_delta_src
  {
    merge_delta_local_src,
    merge_delta_remote_src,
    mergedelta_ancestor_src
  } merge_delta_src;

typedef struct merge_delta
{
  merge_delta_type type;
  merge_delta_src src;
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

static inline merge_delta_type
merge_delta_get_type (merge_delta *delta)
{
  return delta->type;
}

static inline void
merge_delta_set_type (merge_delta *delta, merge_delta_type type)
{
  delta->type = type;
  return;
}

static inline merge_delta_src
merge_delta_get_src (merge_delta *delta)
{
  return delta->src;
}

static inline void
merge_delta_set_src (merge_delta *delta, merge_delta_src src)
{
  delta->src = src;
  return;
}

static inline merge_delta *
merge_delta_create (merge_delta_type type, merge_delta_src src)
{
  merge_delta *d = merge_delta_create_empty();
  merge_delta_set_type (d, type);
  merge_delta_set_src (d, src);
  return d;
}

static inline merge_map *
merge_delta_get_map (merge_delta *delta)
{
  return delta->map;
}

static inline void
merge_delta_set_map (merge_delta *delta, merge_map *map)
{
  delta->map = map;
  return;
}

#endif
