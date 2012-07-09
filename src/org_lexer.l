 /* Org Mode file lexer -*-c-*- */
%{
 /* Org Mode lexer */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "org_parser.h"
#include "doc_elt.h"
#include "org_heading.h"
#include "org_text.h"

int yywrap (yyscan_t scanner);
void yyerror (char const  *);

doc_elt *curr_elt = NULL;
TOKEN curr_type = NOTHING;

%}

 /* Flex Options */
 /* Generate a reentrant parser */
%option reentrant
%option header-file="org_lexer.h"

 /* Rule Start Conditions */
%x heading text
%s property

 /* Start of rules */
%%

 /**
  * Element Break 
  */

[\n]*"\n\n\n" {

  /* Create a temporary data store */
  yyextra = curr_elt;
  TOKEN temp = curr_type;

  /* create th e new element */
  curr_type = ORG_TEXT;
  curr_elt = (doc_elt *) org_text_create_empty ();

  /* add data to the element */
  org_text_strncat ((org_text *) curr_elt, yytext, sizeof (char) * yyleng);

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
  /* Create a temporary data store */
  yyextra = curr_elt;
  TOKEN temp = curr_type;

  /* create the new element */
  curr_elt = (doc_elt *) org_heading_create_empty ();
  curr_type = ORG_HEADING;

  /* add data to the element */
  org_heading_set_level ((org_heading *) curr_elt, yyleng - 1);
  BEGIN (heading);

  /* return the previous element, if there was one */
  if (temp != NOTHING)
    {
      return temp;
    }

}

 /* Grab the rest of the heading element */
<heading>.*"\n"? {

  /* x state, no prev element */
  assert (curr_type == ORG_HEADING);

  /* copy in the data */
  char *c = malloc (sizeof (char)*(yyleng + 1));
  strncpy (c, yytext, yyleng);
  c[yyleng] = '\0';

  org_heading_set_text ((org_heading *) curr_elt, c);

  /*set the element types */
  yyextra = curr_elt;
  curr_type = NOTHING;
  BEGIN (INITIAL);

  return ORG_HEADING;
}

 /**
  * Text 
  */

 /* start of a plain text segment */
^. {
  if (curr_type != ORG_TEXT)
    {
      /* We need to change the token to text.
       * Store the previous token so we can return it @ the end of the rule
       * Create a temporary data store */ 
      TOKEN temp = curr_type;
      yyextra = curr_elt;

      /* create the new element */
      curr_type = ORG_TEXT;
      curr_elt = (doc_elt *) org_text_create_empty ();

      BEGIN(text);
      yymore();

      /* return the previous element, if there was one */
      if (temp != NOTHING)
	{
	  return temp;
	}
    }
  else if (curr_type == ORG_TEXT)
    {
      /* squash together text elements if we can */
      /* add data to the element */
      org_text_strncat ((org_text *) curr_elt, yytext, sizeof (char) * yyleng);
      BEGIN(text);
    }
 }

 /* finish a line */
<text>.*"\n"? {

  /* grab an line of text */
  /* should be guaranteed to have a text element */
  assert (curr_type == ORG_TEXT);

  /* append this line of text */
  //fwrite (yytext, sizeof(char), yyleng, stdout);
  org_text_strncat ((org_text *) curr_elt, yytext, sizeof (char)*yyleng);

  /* Can still match more text for this element */
  /* curr_type == org text */
  BEGIN(INITIAL);
}

 /* a newline apart of the paragraph */
^"\n" {
  if (curr_type == NOTHING)
    {
      curr_type = ORG_TEXT;
      curr_elt = (doc_elt *) org_text_create_empty();
    }
  
  if (curr_type == ORG_TEXT)
    org_text_strncat ((org_text *) curr_elt, "\n", sizeof (char));
  /* does not match anything hopefully */

}

 /* Close the Lexer */
<<EOF>> {
  /* If there is an element beintg defined, return it */
  if (curr_type == NOTHING)
    {
      yyterminate();
    }
  else
    {
      yyextra = curr_elt;
      TOKEN t = curr_type;
      curr_type = NOTHING;
      return t;
    }
}

%%

/* User Code Section */
int
yywrap (yyscan_t scanner)
{
  return 1;
}
