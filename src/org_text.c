#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include "doc_elt.h"
#include "doc_elt_ops.h"
#include "org_text.h"
#include "merge_print_ctxt.h"

struct org_text_ops;

static void
org_text_print_op (doc_elt *self, void *ctxt, doc_stream *out);

static void
org_text_merge_print_op (merge_delta *delta, merge_print_ctxt *ctxt,
			 doc_stream *out);

static int
org_text_is_related_op (doc_elt *a, doc_src a_src, doc_elt *b, doc_src b_src, void *ctxt);

static doc_elt_compare_result
org_text_compare_op (doc_elt *a, doc_src a_src, doc_elt *b, doc_src b_src);

static doc_elt_ops org_text_ops = {
  .print       = &org_text_print_op,
  .merge_print = &org_text_merge_print_op,
  .is_related  = &org_text_is_related_op,
  .compare     = &org_text_compare_op
};

struct org_text
{
  doc_elt  elt;
  char    *text;
  size_t   text_size;
};

/*
 * Constructor, Destructor
 */
org_text *
org_text_create_empty ()
{
  /**
   * @todo Make a better allocation scheme, this is a mess.
   */
  org_text *txt   = malloc (sizeof (org_text));
  doc_elt_set_ops (&(txt->elt), &org_text_ops);
  txt->text_size  = 0;
  txt->text       = realloc (NULL, sizeof (char));
  txt->text[0]    = '\0';
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
  char * t = realloc (self->text, self->text_size + num + 1);
  self->text = t;
  strncat(self->text, str, num);
  self->text_size += num;
  return;
}

char *
org_text_get_text (org_text *self)
{
  return self->text;
}

size_t
org_text_get_text_size (org_text *self)
{
  return self->text_size;
}

/*
 * Document Element Operations
 */
void
org_text_print_op (doc_elt *self, void *ctxt, doc_stream *out)
{
  assert (self->ops == &org_text_ops);

  /**
   * @todo switch fwrite with a doc_stream function.
   */
  fwrite (((org_text *)self)->text, sizeof(char), ((org_text *)self)->text_size, out);
  return;
}

static void
org_text_merge_print_op (merge_delta *delta, merge_print_ctxt *ctxt, doc_stream *out)
{
  /**
   * @todo Implement org_text_merge_print_op. */
  /* For now, just print the one element */
  doc_elt *elt = merge_delta_get_elt (delta);
  org_text_print_op (elt, ctxt, out);
  ctxt->depth++;
  return;
}

static int
org_text_is_related_op (doc_elt *a, doc_src a_src, doc_elt *b, doc_src b_src, void *ctxt)
{
  /* If two text elements have the same text, then they are equal.
   * Use standard string.h's strcmp to achieve this.
   */
  assert (a->ops == &org_text_ops);
  assert (b->ops == &org_text_ops);

  return 0; //( 0 == strcmp (((org_text *)a)->text, ((org_text *)b)->text));
}

static doc_elt_compare_result
org_text_compare_op (doc_elt *a, doc_src a_src, doc_elt *b, doc_src b_src)
{
  /**
   * this function just returns if the two texts are the same or not
   */
  assert (a->ops == &org_text_ops);
  assert (b->ops == &org_text_ops);

  if ( 0 == strcmp (((org_text *)a)->text, ((org_text *)b)->text))
    {
      return elt_compare_same;
    }

  return 1;//elt_compare_different;
}
