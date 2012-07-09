#ifndef MERGE_CHANGE_H
#define MERGE_CHANGE_H

struct merge_delta;
typedef struct merge_delta merge_delta;

/**
 * Indicates a type of structural document change.
 */
typedef enum merge_change
  {
    change_insert,
    change_remove,
    change_none,
    unchanged,
    unmapped,
    mapped,
    matched
  } merge_change;

/**
 * A simple pair of merge_delta and merge_change.
 */
typedef struct merge_delta_change
{
  merge_delta *delta;
  merge_change change;
} merge_delta_change;

#endif
