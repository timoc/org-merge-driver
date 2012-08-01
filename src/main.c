#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <iconv.h>
#include "debug.h"

#include "doc_stream.h"
#include "doc_ref.h"
#include "print.h"
#include "org_document.h"
#include "doc_elt.h"
#include "doc_elt_ops.h"
#include "org_parser.h"
#include "smerger.h"
int
main (int argc, char *argv[])
{
  /* Process command line arguments */
  char *progname = argv[0];
  FILE * out = NULL;
  debug_msg (MAIN, 4, "Staring program\n");
  if (argc < 4)
    {
      printf ("%s: missing operands\n", argv[0]);
    }
  if (argc == 5)
    {
      out =  fopen ( argv[4], "w");
    }
  if (out == NULL)
    {
      debug_msg (MAIN, 4, "Setting output to stdout\n");
      out = stdout;
    }

  FILE *anc_file = fopen ( argv[1], "r");
  if (anc_file != NULL)
    {
      debug_msg (MAIN, 4, "File 1 opened\n");
      FILE *loc_file = fopen (argv[2], "r");
      if (loc_file != NULL)
	{
	  debug_msg (MAIN, 4, "File 2 opened\n");
	  FILE *rem_file = fopen (argv[3], "r");
	  if (rem_file != NULL)
	    {
	      debug_msg (MAIN, 4, "File 3 opened\n");
	      debug_msg (MAIN, 4, "Parsing Files\n\n");
	      org_document *anc = org_parse_file_stream (anc_file, ANC_SRC);
	      org_document *loc = org_parse_file_stream (loc_file, LOC_SRC);
	      org_document *rem = org_parse_file_stream (rem_file, REM_SRC);
#if MAIN_PRINTLEVEL >= 4
	      struct print_ctxt dbgctxt;
	      print_ctxt_init (&dbgctxt);
	      debug_msg (MAIN, 3, "Printing Ancestor\n");
	      org_document_print (anc, &dbgctxt, stderr);
	      debug_msg (MAIN, 3, "Printing Local\n");
	      org_document_print (loc, &dbgctxt, stderr);
	      debug_msg (MAIN, 3, "Printing Remote\n");
	      org_document_print (rem, &dbgctxt, stderr);
#endif
	      debug_msg (MAIN, 4, "Merging Files\n\n");
	      debug_msg (MAIN, 4, "Merging anc and loc\n");
	      merge_ctxt ctxt1;
	      ctxt1.global_smerger = smerger_create ();
	      org_document_merge (anc, loc, &ctxt1);
	      debug_msg (MAIN, 4, "Merging anc and rem\n");
	      merge_ctxt ctxt2;
	      ctxt2.global_smerger = smerger_create ();
	      org_document_merge (anc, rem, &ctxt2);

	      struct print_ctxt ctxt;
	      print_ctxt_init (&ctxt);
	      debug_msg (MAIN, 3, "Printing Ancestor\n\n");
	      org_document_print (anc, &ctxt, stdout);
	      /*
	      debug_msg (MAIN, 4, "Printing Files\n\n");
	      merge_print_ctxt mp_ctxt;
	      mp_ctxt.depth = 0;
	      mp_ctxt.content_conflict_state = 0;
	      mp_ctxt.structural_conflict_state=0;
	      merge_print (mtree, &mp_ctxt, out);
	      */
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
  debug_msg (MAIN, 4, "Closing program\n");
  /* Exit */
  return EXIT_SUCCESS;
}
