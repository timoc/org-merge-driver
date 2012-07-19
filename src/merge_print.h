#ifndef MERGE_PRINT_H
#define MERGE_PRINT_H

#include "merge_map.h"
#include "doc_stream.h"
#include "merge_tree.h"

struct merge_print_ctxt;
typedef struct merge_print_ctxt merge_print_ctxt;

void merge_print (merge_tree *tree, merge_print_ctxt *ctxt, doc_stream *out);

typedef enum merge_conflict
  {
    no_conflict = 0,
    move_conflict = 1 << 0,
    update_conflict = 1 << 1,
    delete_conflict = 1 << 2,
  } merge_conflict;


/**
 * A context for merge_print.
 */
typedef struct merge_print_ctxt
{
  doc_src conflict_src;
  merge_conflict conflict;
  int content_conflict_state;
  int structural_conflict_state;
  int depth; /**< depth, used for testing */
} merge_print_ctxt;

static
void mark_structural_conflict (merge_print_ctxt *ctxt, doc_stream *out)
{
 switch (ctxt->structural_conflict_state)
    {
    case 0:
      doc_stream_puts(">>>\n", out);
      ctxt->structural_conflict_state++;
      break;
    case 1:
      doc_stream_puts("===\n", out);
      ctxt->structural_conflict_state++;
      break;
    case 2:
      doc_stream_puts("<<<\n", out);
      ctxt->structural_conflict_state = 0;
      break;
    }
}
static
void mark_content_conflict (merge_print_ctxt *ctxt, doc_stream *out)
{
  switch (ctxt->content_conflict_state)
    {
    case 0:
      doc_stream_puts(">>>\n", out);
      ctxt->content_conflict_state++;
      break;
    case 1:
      doc_stream_puts("===\n", out);
      ctxt->content_conflict_state++;
      break;
    case 2:
      doc_stream_puts("<<<\n", out);
      ctxt->content_conflict_state = 0;
      break;
    }
}
#endif
