/**
 * @file parse_ctxt.h
 *
 * Defines a context storing state while parsing, and all it's
 * associated functions.  The parse context also stores configuration
 * variables controlling the parse behaviour of the lexer, parser, and
 * the doc_elts.
 */

#ifndef PARSE_CTXT
#define PARSE_CTXT

struct gl_list_impl;
typedef struct gl_list_impl * gl_list_t;

typedef struct parse_ctxt
{
  gl_list_t todo_states;
  size_t current_level;
} parse_ctxt;

/**
 * @brief Initialize a parse context.
 */
void parse_ctxt_init (parse_ctxt *ctxt);

/**
 * @brief Set any unset data to a default value.
 */
void parse_ctxt_set_defaults (parse_ctxt *ctxt);

#endif
