 /* Org Mode file lexer -*-c-*- */
%{
 /* Org Mode lexer */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "doc_elt_util.h"

#include "org_parser.h"
#include "doc_elt.h"
#include "org_heading.h"
#include "org_text.h"

int yywrap (yyscan_t scanner);
void yyerror (char const  *);

void headline_aoeaoe (org_heading *heading, char *string, size_t len, yyscan_t scanner);

%}

 /* Flex Options */
 /* Generate a reentrant parser */
%option reentrant
%option header-file="org_lexer.h"
%option extra-type="struct extra *"

 /* Rule Start Conditions */
%x heading text
 /* %s property */

 /* Start of rules */
%%

 /**
  * Element Break
  */
[\n]*"\n\n\n" {
  debug_msg (LEXER, 4, "Element Break\n");
  /* Create a temporary data store */
  yyextra->elt = yyextra->curr_elt;
  TOKEN temp = yyextra->curr_type;

  /* create a new element */
  yyextra->curr_type = ORG_TEXT;
  yyextra->curr_elt = (doc_elt *) org_text_create_empty (&org_text_ops);
  org_text_initversion ( (org_text *) yyextra->curr_elt, yyextra->src);

  /* add data to the element */
  size_t size = org_text_get_length ((org_text *)yyextra->curr_elt, yyextra->src);
  char * string = org_text_get_text ((org_text *) yyextra->curr_elt, yyextra->src);
  char * new_string = malloc (sizeof (char) * (size + yyleng));
  strncpy (new_string, string, size);
  strncpy ((new_string + size), yytext, yyleng);
  org_text_set_text((org_text *) yyextra->elt, new_string, size + yyleng, yyextra->src);

  /* return the previous element, if there was one */
  if (temp != NOTHING)
    {
      return temp;
    }
}

 /**
  * Heading
  */

 /* start of a heading */
^[*]+" " {
   debug_msg (LEXER, 4, "heading start\n");

   /* Create a temporary data store */
   TOKEN temp = yyextra->curr_type;
   yyextra->elt = yyextra->curr_elt;

  /* create the new element */
  yyextra->curr_elt =  (doc_elt *) org_heading_create_empty (&org_heading_ops);
  yyextra->curr_type = ORG_HEADING;
  org_heading_initversion ((org_heading *)yyextra->curr_elt, yyextra->src);
  org_heading_set_level ((org_heading *) yyextra->curr_elt, yyleng - 1, yyextra->src);
  BEGIN (heading);

  /* return the previous element, if there was one */
  if (yyextra->elt != NULL)
    {
      debug_msg (LEXER, 3, "element return\n");
      return temp;
    }
}

 /* Grab the rest of the heading element */
<heading>.*"\n"? {
  debug_msg (LEXER, 5, "heading finishing line\n");

  /* copy in the data in  */
  char *c = malloc (sizeof (char)*(yyleng) + 2);
  strncpy (c, yytext, yyleng);
  org_heading_set_text ((org_heading *)yyextra->curr_elt, c, yyleng, yyextra->src);

  /* scan the line for the internal values */

  BEGIN (INITIAL);
}

 /**
  * Text
  */

 /* start of a plain text segment */
^. {
  BEGIN(text);
  yymore ();
  debug_msg (LEXER, 5, "text start line\n");
 }

 /* finish a line */
<text>.*"\n"? {
  debug_msg (LEXER, 5, "text finish line\n");

  TOKEN temp = NOTHING;

  /* append this line of text */
  if (yyextra->curr_type != ORG_TEXT)
    {
      /* Create a temporary data store */
      temp = yyextra->curr_type;
      yyextra->elt = yyextra->curr_elt;

      debug_msg (LEXER, 5, "text create new\n");
      /* create a new element */
      yyextra->curr_type = ORG_TEXT;
      yyextra->curr_elt = (doc_elt *) org_text_create_empty (&org_text_ops);
      org_text_initversion ( (org_text *) yyextra->curr_elt, yyextra->src);
    }

  /* add data to the element */
  size_t size = org_text_get_length ((org_text *)yyextra->curr_elt, yyextra->src);
  char * string = org_text_get_text ((org_text *) yyextra->curr_elt, yyextra->src);
  char * new_string = malloc (sizeof (char) * (size + yyleng));
  strncpy (new_string, string, size);
  strncpy ((new_string + size), yytext, yyleng);
  org_text_set_text((org_text *) yyextra->curr_elt, new_string, size + yyleng, yyextra->src);

  BEGIN(INITIAL);

  /* return the previous element, if there was one */
  if (temp != NOTHING)
    {
      debug_msg (LEXER, 3, "element return\n");
      return temp;
    }

  /* Can still match more text for this element */
}

 /* a newline apart of the paragraph */
^"\n" {
  debug_msg (LEXER, 5, "text add newline\n");

  TOKEN temp = NOTHING;

  /* append this line of text */
  if (yyextra->curr_type != ORG_TEXT)
    {
      /* Create a temporary data store */
      temp = yyextra->curr_type;
      yyextra->elt = yyextra->curr_elt;

      debug_msg (LEXER, 5, "text create new\n");
      /* create a new element */
      yyextra->curr_type = ORG_TEXT;
      yyextra->curr_elt = (doc_elt *) org_text_create_empty (&org_text_ops);
      org_text_initversion ( (org_text *) yyextra->curr_elt, yyextra->src);
    }

  /* add data to the element */
  size_t size = org_text_get_length ((org_text *)yyextra->curr_elt, yyextra->src);
  char * string = org_text_get_text ((org_text *) yyextra->curr_elt, yyextra->src);
  char * new_string = malloc (sizeof (char) * (size + yyleng));
  strncpy (new_string, string, size);
  strncpy ((new_string + size), yytext, yyleng);
  org_text_set_text((org_text *) yyextra->curr_elt, new_string, size + yyleng, yyextra->src);

  BEGIN(INITIAL);

  /* return the previous element, if there was one */
  if (temp != NOTHING)
    {
      debug_msg (LEXER, 3, "element return\n");
      return temp;
    }
 /* Can still match more text for this element */
}

 /* Close the Lexer */
<<EOF>> {
  debug_msg (LEXER, 5, "EOF\n");
  if (yyextra->curr_elt != NULL)
    {
      /* If there is an element beintg defined, return it */
      yyextra->elt = yyextra->curr_elt;
      yyextra->curr_elt = NULL;
      debug_msg (LEXER, 5, "EOF return element\n");
      return yyextra->curr_type;
    }
  else
    {
      debug_msg (LEXER, 4, "EOF terminate lexer\n");
      yyterminate();
    }
}

%%

  /* User Code Section */
int
yywrap (yyscan_t scanner)
{
  return 1;
}

bool
is_todo_state (substr *s)
{
  return false;
}

/**
 * @brief Parse a titile line, setting all the propper substrings
 */
parse_title_line (org_heading * h, char * line, size_t len)
{
  int i = 0;

  size_t last_len = 0;
  substr next_substr;

  /* set all the substrings to len 0 */

  /* skip all starting blank space*/
  while ( i < len)
    {
      if (line[i] != ' ')
        break;
    }

  /* get the next word */
  next_substr.string = (line + i);
  last_len = i;
  while ( i < len)
    {
      if (line[i] == ' ')
        break;
    }
  next_substr.length = i - last_len;

  /* check what the last word was */
  if (is_todo_state (&next_substr))
    {
      // implement this
      //h->todo = s;
      /* get the next word */
      next_substr.string = (line + i);
      last_len = i;
      while ( i < len)
        {
          if (line[i] == ' ')
            break;
        }
      next_substr.length = i - last_len;
    }

  /* assume that the next substr is either text or :tags: */
  bool exit = false;
  while ( i < len)
    {

      if (next_substr.string[0] == ':')
	{
	  /* grab all characters untill there is no tag */
	}
      else
	{
	  /* grab all the characters, assuming its heading text */
	}

      /* get the next word */
      next_substr.string = (line + len);
      last_len = len;
      while ( i < len)
	{
	  if (line[i] != ' ')
	    break;
	}

      next_substr.length = len - last_len;
    }
}
