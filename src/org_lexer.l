/* Org Mode file lexer -*-c-*- */
%{
/* Org Mode lexer */
#include <stdio.h>
#include "org_parser.h"

int yywrap();

/* extern int yylex (void); */
extern void yyerror (char const  *);
%}

%option header-file="org_lexer.h"

/* %option reentrant */
/* %option bison-bridge */
/* %option bison-locations */
/* %option yylineno */
/* %option warn nodefault */
/* %option noyywrap */
/* %option bison-bridge */

%x finish_line

%%
"\n\n\n" { /* New element detection */
  return NEW_ELEMENT;
}

"\n" {
  //printf ("single newline\n");
  return NEW_LINE;
}

^[*]+" " { /* Heading Recognition */
  BEGIN(finish_line);
  return HEADING;
}

^"- " {/* Plain List */
  BEGIN(finish_line);
  return LIST;
}

^[0-9]+[).]?" " {/* Numbered List */
  BEGIN(finish_line);
  return NUMBERED_LIST;
}

^. {/* Start of a plain line */
  BEGIN(finish_line);
  yymore();
}

<finish_line>.* {
  BEGIN(INITIAL);
  return TEXT;
}
%%

int
yywrap (void)
{
  printf ("End of File \n");
  return 1;
}
