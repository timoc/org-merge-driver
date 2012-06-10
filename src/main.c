#include <stdio.h>
#include <ctype.h>
/*
#include "Parser.h"
#include "Lexer.h"
*/
#include "org_parser.h"
#include "org_lexer.h"

char *progname;
//double yylval;

main( argc, argv )
char *argv[];
{
  progname = argv[0];
  if (argc > 1)
    {
      yyin = fopen (argv[1], "r");
    }
  if (yyin == 0)
    yyin = stdin;
  //yylex();

  yyparse();
}

yyerror( s )
char *s;
{
  fprintf( stderr ,"%s: %s\n" , progname , s );
}
