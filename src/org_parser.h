/**
 * @file org_parser.h
 */

#ifndef ORG_PARSER_H
#define ORG_PARSER_H

#include <stdio.h>
//#include "org_lexer.h"

#include "org_document.h"
#include "parse_ctxt.h"

struct doc_elt;
typedef struct doc_elt doc_elt;



/**
 * @file org_parser.h
 * @brief Implement an Org mode file parser.
 */

/**
 * @brief parse a FILE stream into a doc_tree.
 */
org_document *org_parse_file_stream (FILE * file, doc_src src, parse_ctxt *ctxt);

/**
 * Token declaration for the lexer & parser.
 */
typedef enum TOKEN
  {
    T_QUIT = 0,         /* signal to quit from the lexer */
    T_NOTHING,          /* return nothing (no element) */
    T_ORG_HEADING,      /* a heading */
    T_ORG_TEXT,         /* regular text under a heading */
    T_ORG_PROPERTY,     /* a property */
    T_ORG_DRAWER_BEGIN, /* beginning of a drawer */
    T_ORG_DRAWER_END    /* end of a drawer */
  } TOKEN;

struct extra
{
  doc_elt    *elt;
  doc_elt    *curr_elt;
  TOKEN       curr_type;
  void       *data;
  doc_src     src;
  parse_ctxt *ctxt;
};
#endif
