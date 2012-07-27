#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "org_parser.h"
#include "org_lexer.h"

#include "doc_elt.h"
#include "org_document.h"
#include "org_heading.h"
#include "org_text.h"

static void rec_parse_document (yyscan_t scanner, org_document *this);

static doc_elt *rec_parse_heading(yyscan_t scanner, org_heading *dc, int level);

/* This parser needs some serious love.  Right now it assumes that the
 * only type of elements which can be nested are headings.
 */
org_document *
org_parse_file_stream (FILE * file, doc_src src)
{
  assert (file);
  debug_msg (PARSER, 3, "Parsing File\n");

  /* Initialize flex data structures */
  yyscan_t scanner;
  int err = 0;
  err = yylex_init (&scanner);

  if (err == ENOMEM)
    {
      /* Memory allocation error, abort execution. */
      return NULL;
    }
  else if (err == EINVAL)
    {
      /* Invalid yylex_init argument, abort execution. */
      return NULL;
    }

  /* Set the input stream of the lexer */
  yyset_in (file, scanner);

  /* set up the extra field */
  struct extra e;
  e.elt = NULL;
  e.curr_elt = NULL;
  e.curr_type = NOTHING;
  e.src = src;
  yyset_extra (&e, scanner);

  /* Initialize doc_tree */
  org_document *document = org_document_create_empty (&org_document_ops);

  /* call the recursive function */
  rec_parse_document (scanner, document);

  /* Destroy scanner */
  yylex_destroy (scanner);
  debug_msg (PARSER, 3, "Finished Parsing File\n");
  return document;
}

static void
rec_parse_document (yyscan_t scanner, org_document *this)
{
  /* 1. get the text element
   * 2 check type
   * 2.1 Non children type: add it to the list of elements
   * 2.2 Children type, check the level
   * 2.2.1 If same level, or higher, return the node
   * 2.2.2 If its a lower level, call this function on it
   */

  bool exit = false;
  doc_elt * ret = 0;
  //int last_level = level;

  /* Get the next element from the scanner */
  TOKEN tok = yylex (scanner);
  doc_elt * elt = yyget_extra (scanner)->elt;
  doc_src src = yyget_extra (scanner) -> src;

#if PARSER_PRINTLEVEL > 4
  //doc_elt_print (elt, NULL, stderr);
#endif

  /* Parse the file */
  while(!exit)
    {
      if (tok == NOTHING)
	{
	  debug_msg (PARSER, 3, "Got Nothing\n");
	  /* do nothing */
	}
      else if (tok == ORG_HEADING)
	{
	  int next_level = org_heading_get_level ((org_heading *) elt, src);
	  if (next_level <= 0)
	    {
	      debug_msg (PARSER, 3, "heading with level <= 0\n");
	      assert (0);
	      return;
	    }
	  else
	    {
	      debug_msg (PARSER, 3, "Adding Sub-Heading\n");
	      /* next level is at least more than this one */
	      org_document_add_heading_last (this, src, (org_heading *) elt);
	      elt = (doc_elt *)
		rec_parse_heading(scanner, (org_heading *) elt, next_level);
	    }
	}
      else if (tok == ORG_TEXT)
	{
	  debug_msg (PARSER, 3, "Got Text\n");
	  /* eat up all text elements below this one */
	  org_document_add_text_last (this, src, (org_text *) elt);

	  /* Get the next element from the scanner */
	  tok = yylex (scanner);
	  elt = yyget_extra (scanner) -> elt;
	}

      if (tok == QUIT || elt == NULL)
	{
	  /* break if at the end of the file */
	  ret = NULL;
	  exit = true;
	  tok = QUIT;
	  elt = NULL;
	}
    }
  return;
}

static doc_elt *
rec_parse_heading(yyscan_t scanner, org_heading *this, int this_level)
{
  /* 1. get the text element
   * 2 check type
   * 2.1 Non children type: add it to the list of elements
   * 2.2 Children type, check the level
   * 2.2.1 If same level, or higher, return the node
   * 2.2.2 If its a lower level, call this function on it
   */

  bool exit = false;
  doc_elt * ret = 0;

  /* Get the next element from the scanner */
  TOKEN tok = yylex (scanner);
  doc_elt * elt = yyget_extra (scanner)->elt;
  doc_src src = yyget_extra (scanner) -> src;

#if PARSER_PRINTLEVEL > 4
  //doc_elt_print (elt, NULL, stderr);
#endif

  /* Parse the file */
  while(!exit)
    {
      if (tok == NOTHING)
	{

	  debug_msg (PARSER, 3, "Got Nothing\n");
	  /* do nothing */
	}
      else if (tok == ORG_HEADING)
	{
	  int next_level = org_heading_get_level ((org_heading *)elt, src);
	  //int next_level = 1;
	  if (next_level <= this_level)
	    {
	      debug_msg (PARSER, 3, "-return heading\n");
	      return elt;
	    }
	  else
	    {
	      debug_msg (PARSER, 3, "Adding Sub-Heading\n");
	      /* next level is at least more than this one */
	      org_heading_add_subheading_last (this, src, (org_heading *) elt);
	      elt =
		rec_parse_heading(scanner, (org_heading *)elt, next_level);
	    }
	}
      else if (tok == ORG_TEXT)
	{
	  debug_msg (PARSER, 3, "Got Text\n");

	  /* eat up all text elements below this one */
	  org_heading_add_subtext_last (this, src, (org_text *) elt);
	  /* Get the next element from the scanner */
	  tok = yylex (scanner);
	  elt = yyget_extra (scanner)-> elt;
	}

      if (tok == QUIT || elt == NULL)
	{
	  /* break if at the end of the file */
	  ret = NULL;
	  exit = true;
	  tok = QUIT;
	  elt = NULL;
	}
    }
  return ret;
}
