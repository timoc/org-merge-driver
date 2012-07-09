#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <iconv.h>

#include "doc_stream.h"
#include "org_parser.h"
#include "merge_tree.h"
#include "doc_tree.h"
#include "merge_print.h"
#include "doc_merge.h"
#include "merge_print_ctxt.h"

int
main (int argc, char *argv[])
{
  /* Process command line arguments */
  char *progname = argv[0];
  FILE * out = NULL;

  if (argc < 4)
    {
      printf ("%s: missing operands", argv[0]);
    }
  if (argc == 5)
    {
      out =  fopen ( argv[4], "w");
    }
  if (out == NULL)
    {
      out = stdout;
    }

  FILE *anc_file = fopen ( argv[1], "r");
  if (anc_file != NULL)
    {
      FILE *loc_file = fopen (argv[2], "r");
      if (loc_file != NULL)
	{
	  FILE *rem_file = fopen (argv[3], "r");
	  if (rem_file != NULL)
	    {
	      doc_tree *anc_dtree = org_parse_file_stream (anc_file);
	      doc_tree *loc_dtree = org_parse_file_stream (loc_file);
	      doc_tree *rem_dtree = org_parse_file_stream (rem_file);

	      merge_tree *mtree = create_merge_tree(anc_dtree, loc_dtree, rem_dtree);

	      merge_print_ctxt mp_ctxt;
	      mp_ctxt.depth = 0;
	      merge_print (mtree, &mp_ctxt, out);

	      fclose (rem_file);
	    }
	  fclose (loc_file);
	}
      fclose(anc_file);
    }


  if (out != stdout && out != NULL)
    {
      fclose (out);
    }

  /* Exit */
  return EXIT_SUCCESS;
}
