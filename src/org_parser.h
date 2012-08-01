/*
 *
 */

#ifndef ORG_PARSER_H
#define ORG_PARSER_H

#include <stdio.h>
//#include "org_lexer.h"

#include "org_document.h"

struct doc_elt;
typedef struct doc_elt doc_elt;

/**
 * @file org_parser.h
 * @brief Implement an Org mode file parser.
 */

/**
 * @brief parse a FILE stream into a doc_tree.
 */
org_document *org_parse_file_stream (FILE * file, doc_src src);

/**
 * Token declaration for the lexer & parser.
 */
typedef enum TOKEN
  {
    T_QUIT = 0,
    T_NOTHING,
    T_ORG_HEADING,
    T_ORG_TEXT         /* regular text under a heading */
  } TOKEN;

struct extra
{
  doc_elt *elt;
  doc_elt *curr_elt;
  TOKEN curr_type;
  void *data;
  doc_src src;
};
#endif
