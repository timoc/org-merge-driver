#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "doc_elt.h"
#include "doc_elt_ops.h"
#include "org_text.h"

static void org_text_print_op (doc_elt *self, void *ctxt, doc_stream *out);
static void org_text_merge_print_op (merge_delta *delta, void *ctxt, doc_stream *out);
static bool org_text_compare_op (doc_elt *a, doc_src, doc_elt *b, doc_src, void *ctxt);

typedef struct org_text
{
  doc_elt  elt;
  char    *text;
  size_t   text_size;
} org_text;

static doc_elt_ops org_text_ops = {
  .print       = org_text_print_op,
  .merge_print = org_text_merge_print_op,
  .compare     = org_text_compare_op 
};

/* 
 * Constructor, Destructor 
 */
org_text *
org_text_create_empty ()
{
  org_text *txt   = malloc (sizeof (org_text));
  txt->elt.ops    = &org_text_ops;
  txt->text_size  = 1;
  txt->text       = malloc (sizeof (char));
  return txt;
}

void
org_text_free (org_text *self)
{
  free (self->text);
  free (self);
}

/* 
 * Modifier functions 
 */
void
org_text_strncat (org_text *self, char *str, size_t num)
{
  self->text = realloc (self->text, self->text_size + num);
  strncpy(self->text + self->text_size -1, str, num);
  self->text_size += num;
  self->text[self->text_size] = 0;
  return;
}

/* 
 * Document Element Operations
 */
static void org_text_print_op (doc_elt *self, void *ctxt, doc_stream *out)
{
  assert (self->ops == &org_text_ops);
  doc_stream_puts (((org_text *)self)->text, out);
}

static void org_text_merge_print_op (merge_delta *delta, void *ctxt, doc_stream *out)
{
  /**
   * @todo implement org_text_merge_print_op
   */
}

static bool org_text_compare_op (doc_elt *a, doc_src a_src, doc_elt *b, doc_src b_src, void *ctxt)
{
  /* If two text elements have the same text, then they are equal.
   * Use standard string.h's strcmp to achieve this.
   */
  assert (a->ops == &org_text_ops);
  assert (b->ops == &org_text_ops);

  if ( strcmp (((org_text *)a)->text, ((org_text *)b)->text) == 0)
    {
      return true;
    }
  else
    {
      return false;
    }

  /* unreachable */
  return false;
}
