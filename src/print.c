#include <stdlib.h>
#include "doc_stream.h"
#include "print_ctxt.h"
#include "print.h"

static const char *start_mark  = ">>> ";
static const char *middle_mark = "=== ";
static const char *end_mark    = "<<< ";

void
enter_structural_conflict (print_ctxt *ctxt, conflict_state state,
			   char* msg, doc_stream *out)
{
  /* wrap up the conflicts, print a message on the last conflict
   * marker, if there is one */

  if ( ctxt->structure_conflict != state )
    {
      enter_content_conflict (ctxt, no_conflict, NULL, out);
    }
  else
    return;

  while (ctxt->structure_conflict != state )
    {
      /*conflict wrap up */
      switch (ctxt->structure_conflict)
	{
	case no_conflict:
	  doc_stream_puts (start_mark, out);
	  break;
	case local_side:
	  doc_stream_puts (middle_mark, out);
	  break;
	case remote_side:
	  doc_stream_puts (end_mark, out);
	  break;
	}
      ctxt->structure_conflict ++;

      if (ctxt->structure_conflict == 3)
	ctxt->structure_conflict = 0;

      if (ctxt->structure_conflict != state)
	doc_stream_putc ('\n', out);
    }

  if (msg != NULL)
    {
      doc_stream_puts (msg, out);
    }
  else
    {
      doc_stream_putc ('\n', out);
    }

  return;
}

void
enter_content_conflict (print_ctxt *ctxt, conflict_state state,
			char* msg, doc_stream *out)
{
  if (ctxt->content_conflict == state)
    return;

  while ( ctxt->content_conflict != state  )
    {
      /*conflict wrap up */
      switch (ctxt->content_conflict)
	{
	case no_conflict:
	  doc_stream_puts (start_mark, out);
	  break;
	case local_side:
	  doc_stream_puts (middle_mark, out);
	  break;
	case remote_side:
	  doc_stream_puts (end_mark, out);
	  break;
	}
      ctxt->content_conflict ++;

      if (ctxt->content_conflict == 3)
	ctxt->content_conflict = 0;

      if (ctxt->content_conflict != state)
	doc_stream_putc ('\n', out);

    }

  if (msg != NULL)
    {
      doc_stream_puts (msg, out);
    }
  else
    {
      doc_stream_putc ('\n', out);
    }

  return;
}

