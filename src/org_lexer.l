/* Org Mode file lexer -*-c-*- */

%{
/* Org Mode lexer */
#include <stdio.h>
#include <stdlib.h>

int yywrap (yyscan_t yyscanner);
extern void yyerror (char const  *);
%}

/* Flex Options */
/* Generate a reentrant parser */
%option reentrant
%option header-file="org_lexer.h"

/* Rule Start Conditions */
%x finish_line heading plain_list properties uid num_list
%s property

%%

[\n]*"\n\n\n" { /* New element detection */
   printf ( "element break; %d\n", yyleng);
   //return NEW_ELEMENT;
}

"\n" {
  //printf ("single newline\n");
  //return NEW_LINE;
}

^[*]+" " { /* Heading Recognition */
  printf ("heading level: %d, text:", yyleng -1);
  BEGIN (heading);
  //return HEADING;
}

<heading>.* {
  fwrite (yytext, sizeof(char), yyleng, stdout);
  printf ("\n");
  BEGIN (property);
}

<property>[ ]*":PROPERTIES:" { /* Property Drawer */
  printf ("PRORERTIES\n");
  BEGIN (properties);
}

<properties>[ ]*":END:" {
  printf ("Drawer end\n");
  BEGIN (INITIAL);
}

<properties>[ ]*":ID " { /*UID Property */
  printf ("UID: ");
  BEGIN (uid);
}

<uid>.*":" {
  fwrite (yytext, sizeof(char), yyleng-1, stdout);
  printf ("\n");
  BEGIN (properties);
}

^[ ]*"- " {/* Plain List */
  printf ("plain list: ");
  BEGIN (plain_list);
  //return LIST;
}

<plain_list>.* {
  fwrite (yytext, sizeof(char), yyleng, stdout);
  printf ("\n");
  BEGIN (INITIAL);
}

^[ ]*[0-9]+[).]?" " {/* Numbered List */
  printf ("num list: %d, ", atoi(yytext));
  BEGIN(num_list);
  //return NUMBERED_LIST;
}

<num_list>.* {
  fwrite (yytext, sizeof(char), yyleng, stdout);
  printf ("\n");
  BEGIN (INITIAL);
}

^. {/* Start of a plain line */
  BEGIN(finish_line);
  yymore();
}

<finish_line>.* {
  BEGIN(INITIAL);
  //return TEXT;
}

%%

int
yywrap (yyscan_t yyscanner)
{
  printf ("End of File \n");
  return 1;
}
