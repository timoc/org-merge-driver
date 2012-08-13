/**
 * @file print_ctxt.h
 *
 * Defines a context used during element printing and all it's
 * associated functions.
 */

#ifndef PRINT_CTXT_H
#define PRINT_CTXT_H
#include "print.h"
#include "stdbool.h"

/**
 * @brief A context for printing doc_elt's in a tree.
 */
typedef struct print_ctxt
{
  int            depth;              /*< The current depth in a document tree */
  size_t         rmargin;            /*< The column of the doc's right margin */
  size_t         tab_width;          /*< The column width of a tab character  */
  bool           use_tabs;           /*< Should generated output use tabs?    */
  print_state    print_state;        /*< the current printing mode            */
  bool           nested_conflicts;   /*< if there are nested conflicts        */
  conflict_state structure_conflict; /*< the current state of conflicts       */
  conflict_state content_conflict;   /*< the current state of conflicts       */
  bool           conflict_occurred;   /* IF a conflict occured                 */
} print_ctxt;

/**
 * @brief Create and initialize a print context.
 */
print_ctxt* print_ctxt_create_empty (void);

/**
 * @brief Initialize a print_ctxt.
 */
void print_ctxt_init (print_ctxt *ctxt);

#endif
