/*
 *
 */

#ifndef ORG_PARSER_H
#define ORG_PARSER_H

#include <stdio.h>
#include "doc_tree.h"

struct doc_elt;
typedef struct doc_elt doc_elt;

/**
 * @file org_parser.h
 * @brief Implement an Org mode file parser.
 */

/**
 * @brief parse a FILE stream into a doc_tree.
 */
doc_tree *org_parse_file_stream (FILE * file);

/**
 * Token declaration for the lexer & parser.
 */
typedef enum TOKEN 
  {
    QUIT = 0,
    NOTHING,
    ORG_HEADING,
    ORG_TEXT         /* regular text under a heading */
  } TOKEN;

#define YYEXTRATYPE doc_elt*

#endif
