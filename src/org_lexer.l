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

org_heading *curr_elt = NULL;
%}

 /* Flex Options */
 /* Generate a reentrant parser */
%option reentrant
%option header-file="org_lexer.h"

 /* Rule Start Conditions */
%x heading text

 /* Start of rules */
%%

 /**
  * Heading
  */

 /* start of a heading */
^[*]+" " {
   debug_msg (LEXER, 5, "heading start\n");
  /* Create a temporary data store */
  yyextra = curr_elt;

  /* create the new element */
  curr_elt =  org_heading_create_empty ();

  /* add data to the element */
  org_heading_set_level ((org_heading *) curr_elt, yyleng - 1);
  BEGIN (heading);

  /* return the previous element, if there was one */
  if (yyextra != NULL)
    {
      debug_msg (LEXER, 4, "heading return\n");
      return ORG_HEADING;
    }
}

 /* Grab the rest of the heading element */
<heading>.*"\n"? {
  debug_msg (LEXER, 5, "heading finishing line\n");
  /* copy in the data */
  char *c = malloc (sizeof (char)*(yyleng + 1));
  strncpy (c, yytext, yyleng);
  c[yyleng] = '\0';

  org_heading_set_text (curr_elt, c);

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
  /* append this line of text */
  //fwrite (yytext, sizeof(char), yyleng, stdout);
  if (curr_elt == NULL)
    {
      debug_msg (LEXER, 5, "text create new heading\n");
      curr_elt = org_heading_create_empty ();
      org_heading_set_level (curr_elt, 0);
    }

  org_text_strncat (org_heading_get_text_elt (curr_elt), yytext, sizeof (char)*yyleng);

  /* Can still match more text for this element */
  /* curr_type == org text */
  BEGIN(INITIAL);
}

 /* a newline apart of the paragraph */
^"\n" {
  debug_msg (LEXER, 5, "text add newline\n");
 if (curr_elt == NULL)
    {
      debug_msg (LEXER, 5, "text newline create new heading\n");
      curr_elt = org_heading_create_empty ();
      org_heading_set_level (curr_elt, 0);
    }
  org_text_strncat (org_heading_get_text_elt (curr_elt), yytext, sizeof (char)*yyleng);

}

 /* Close the Lexer */
<<EOF>> {
  debug_msg (LEXER, 5, "EOF\n");
  if (curr_elt != NULL)
    {
      /* If there is an element beintg defined, return it */
      yyextra = curr_elt;
      curr_elt = NULL;
	debug_msg (LEXER, 5, "EOF return something\n");
      return ORG_HEADING;
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
