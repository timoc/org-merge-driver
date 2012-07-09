#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "ltree.h"
#include "doc_tree.h"

#include "org_lexer.h"
#include "org_parser.h"

#include "doc_elt.h"
#include "org_heading.h"
#include "org_text.h"

static doc_elt *
rec_parse_heading(yyscan_t scanner, doc_node *dc, int level);

/* This parser needs some serious love.  Right now it assumes that the
 * only type of elements which can be nested are headings.
 */

doc_tree *
org_parse_file_stream (FILE * file)
{
  assert (file);

  /* Initialize flex data structures */
  yyscan_t scanner;
  int err = 0;
  err = yylex_init (&scanner);

  if (err == ENOMEM) 
    {
      /* Memory allocation error.
       * Abort execution. */
    }
  else if (err == EINVAL)
    {
      /* Invalid yylex_init argument 
       * Abort execution. */
    }

  /* Set the input stream of the lexer */
  yyset_in (file, scanner);

  /* Initialize doc_tree */
  doc_tree *dtree = doc_node_create_empty ();

  /* call the recursive function */
  doc_elt *ret = 
    rec_parse_heading (scanner, dtree , 0);

  assert (ret == NULL);
  
  bool exit = false;

  /* Destroy scanner */
  yylex_destroy (scanner);
  return dtree;
}

static doc_elt *
rec_parse_heading(yyscan_t scanner, doc_node *this, int this_level)
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
  doc_elt * elt = yyget_extra (scanner);
  //doc_elt_print (elt, NULL, stdout);

  /* Parse the file */ 
  while(!exit)
    {
      if (tok == NOTHING)
	{
	  /* do nothing */
	}
      else if (tok == ORG_HEADING)
	{
	  int next_level = org_heading_get_level ((org_heading *)elt);

	  if (next_level <= this_level)
	    {
	      return elt;
	    }
	  else
	    {
	      /* next level is at least more than this one */
	      doc_node *d = doc_node_create_empty ();
	      doc_node_set_elt (d, elt);
	      doc_node_add_child_last (this, d);
	      elt =
		rec_parse_heading(scanner, d, next_level);
	    }
	}
      else if (tok == ORG_TEXT)
	{
	  /* eat up all text elements below this one */
	  doc_node * child = doc_node_create_empty ();
	  doc_node_set_elt (child, elt);
	  doc_node_add_child_last (this, child);

	  /* Get the next element from the scanner */
	  tok = yylex (scanner);
	  elt = yyget_extra (scanner);
	}
      else
	{
	  assert(0);
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
