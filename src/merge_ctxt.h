/**
 * @file merge_ctxt.h
 *
 * Defines a context structure containing information used during the
 * matching and merging process, along with all associated functions.
 */

#ifndef MERGE_CTXT_H
#define MERGE_CTXT_H

struct gl_list_impl;
typedef struct gl_list_impl * gl_list_t;

struct smerger;
typedef struct smerger smerger;

typedef struct merge_ctxt
{
  smerger *global_smerger;
  gl_list_t priorities;
} merge_ctxt;

/**
 * @brief Initialize a merge context.
 */
void merge_ctxt_init (merge_ctxt *ctxt);

/**
 * @brief Free a merge context and all it's data.
 */
void merge_ctxt_free (merge_ctxt *ctxt);

/**
 * @brief Set any unset values to their defaults.
 */
void merge_ctxt_set_defaults (merge_ctxt *ctxt);

#endif
