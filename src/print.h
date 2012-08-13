/**
 * @file print.h
 * @brief the print library
 */

#ifndef PRINT_H
#define PRINT_H
#include "doc_stream.h"

struct print_ctxt;
typedef struct print_ctxt print_ctxt;

/**
 * @todo Find a way to deal with advancing the print mode with the
 * conflict markers
 */

/**
 * @brief A context for the current printing mode.
 *
 * It acts as a request to print only certain information.  It does
 * not imply what side of a conflict we are printing.
 */
typedef enum print_state
  {
    print_merged,
    print_remote,
    print_local,
    print_ancestor
  } print_state;

/**
 * @brief The current state of conflict markers.
 */
typedef enum conflict_state
  {
    no_conflict = 0,
    local_side  = 1,
    remote_side = 2
  } conflict_state;

/**
 * @brief enter a certain part of a structural conflict.
 *
 * This function enters a structural conflict.  If it is already in
 * the correct state, it will do nothing.  It will wrap up any content
 * conflicts before switching
 */
void enter_structural_conflict (print_ctxt *ctxt, conflict_state state,
				char* msg, doc_stream *out);

/**
 * @brief enter a certain part of a content conflict.
 *
 * This function enters a content conflict.  If it is already in the
 * correct state, it will do nothing.
 */
void enter_content_conflict (print_ctxt *ctxt, conflict_state state,
			     char* msg, doc_stream *out);

#endif /* PRINT_H */
