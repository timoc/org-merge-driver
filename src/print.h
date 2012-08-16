/**
 * @file print.h
 * @brief the print library
 */

#ifndef PRINT_H
#define PRINT_H
#include "doc_stream.h"

#include "print_ctxt.h"

/**
 * @todo Find a way to deal with advancing the print mode with the
 * conflict markers
 */

/**
 * @brief enter a certain part of a structural conflict.
 *
 * This function enters a structural conflict.  If it is already in
 * the correct state, it will do nothing.  It will wrap up any movement
 * conflicts before switching
 */
void enter_structural_conflict (print_ctxt *ctxt, conflict_state state,
				char* msg, doc_stream *out);

/**
 * @brief enter a certain part of a movement conflict.
 *
 * This function enters a structural conflict. If it is already in the
 * correct state, it will do nothing. It will wrap up any content
 * conflicts before switching. Movement Conflicts are assumed to be
 * nestable between parents and children. It is up to the parent to
 * ensure that the print ctxt is set up for this.
 */
void enter_movement_conflict (print_ctxt *ctxt, conflict_state state,
                         char* msg, doc_stream *out);

/**
 * @brief enter a certain part of a content conflict.
 *
 * This function enters a content conflict.  If it is already in the
 * correct state, it will do nothing.
 */
void enter_content_conflict (print_ctxt *ctxt, conflict_state state,
			     char* msg, doc_stream *out);

static inline conflict_state print_ctxt_get_content_state (print_ctxt *ctxt);

static inline conflict_state print_ctxt_get_structure_state (print_ctxt *ctxt);

static inline conflict_state print_ctxt_get_movement_state (print_ctxt *ctxt);
static inline void print_ctxt_set_movement_state (print_ctxt *ctxt, conflict_state state);

/*
 * Implementation
 */

static inline conflict_state
print_ctxt_get_content_state (print_ctxt *ctxt)
{
  return ctxt->structure_conflict;
}

static inline conflict_state
print_ctxt_get_structure_state (print_ctxt *ctxt)
{
  return ctxt->content_conflict;
}

static inline conflict_state
print_ctxt_get_movement_state (print_ctxt *ctxt)
{
  return ctxt->movement_conflict;
}

static inline void
print_ctxt_set_movement_state (print_ctxt *ctxt, conflict_state state)
{
  ctxt->movement_conflict = state;
}

#endif /* PRINT_H */
