#include <stdio.h>
#include <ctype.h>
#include <iconv.h>
#include "org_lexer.h"

int
main (int argc, char *argv[])
{
  /* Process command line arguments */

  char *progname = argv[0];


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

  /* If a file path was passed, open that file and utilize it as the
     flex source */
  if (argc > 1)
    {
      yyset_in (fopen (argv[1], "r"), scanner);
    }

  /* If no file path is passed, use stdin as flex source */
  if (yyget_in (scanner) == NULL)
    {
      yyset_in (stdin, scanner);
    }

  /* Establish source encoding */
  /* Currently, assume source encoding is utf-8 */
  iconv_t iconv_table = iconv_open ("UTF-8", "UTF-8");



  yylex (&scanner);
  yylex_destroy (&scanner);
  iconv_close (&iconv_table);

  /* Close yyin */
  fclose (yyget_in(scanner));
}
