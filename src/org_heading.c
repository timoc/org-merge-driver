/**
 * @file org_heading.c
 */

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "merge_map.h"
#include "doc_tree.h"
#include "merge_tree.h"
#include "doc_elt.h"
#include "doc_elt_ops.h"
#include "org_heading.h"
#include "merge_print_ctxt.h"

/* Forward Declarations */
struct org_heading_ops;

static void org_heading_print_op (doc_elt *self, void *context, doc_stream *out);

static void org_heading_merge_print_op (merge_delta *delta, merge_print_ctxt *ctxt,
					doc_stream *out);

static int org_heading_is_related_op (doc_elt *local, doc_src sl,
				      doc_elt *remote, doc_src s2,
				      void *context);
static doc_elt_compare_result
org_heading_compare_op (doc_elt *a, doc_src a_src, doc_elt *b, doc_src b_src);

/* Declaration of org_element operations table */
static struct doc_elt_ops org_heading_ops = {
  .print        = &org_heading_print_op,
  .merge_print  = &org_heading_merge_print_op,
  .is_related   = &org_heading_is_related_op,
  .compare      = &org_heading_compare_op
};

/* Core org_heading struct */

struct org_heading
{
  doc_elt elt;
  int level;
  char *heading_text;
};

/* Constructor, Destructor */
org_heading *
org_heading_create_empty ()
{
  org_heading *h = malloc (sizeof (org_heading));
  h->heading_text = NULL;
  doc_elt_set_ops (&(h->elt), &org_heading_ops);
  return h;
}

void
org_heading_free (org_heading *self)
{
  free (self);
}

/* doc_elt interface */
static void
org_heading_print_op (doc_elt *elt, void *ctxt, doc_stream *out)
{
  int i = 0;
  org_heading * self = (org_heading *) elt;
  int level = self->level;
  // print the stars
  for (i = 0; i < level; i++)
    {
      doc_stream_putc ('*', out);
    }
  // assume that if there was a heading, there was a space
  doc_stream_putc (' ', out);
  // print the text
  if (self->heading_text != NULL)
    {
      doc_stream_puts (self->heading_text, out);
    }
  return;
}

static void
org_heading_merge_print_op (merge_delta *delta, merge_print_ctxt *ctxt, doc_stream *out)
{
  assert (delta != NULL);
  doc_src src = merge_delta_get_doc_src (delta);
  merge_map *map = merge_delta_get_map (delta);
  assert (map != NULL);

  merge_change c_anc, c_loc, c_rem;
  c_anc = merge_map_get_change (map, src_ancestor);
  c_loc = merge_map_get_change (map, src_local);
  c_rem = merge_map_get_change (map, src_remote);

  assert (c_anc == mapped || c_anc == unmapped);
  assert (c_loc == mapped || c_loc == unmapped);
  assert (c_rem == mapped || c_rem == unmapped);

  merge_delta *d_anc, *d_loc, *d_rem;
  org_heading *anc, *loc, *rem;

  bool is_updated (org_heading *new, org_heading *old)
  {
    return (0 != strcmp (new->heading_text, old->heading_text));
  }

  if (c_anc == mapped)
    {
      if (c_loc == unmapped)
	{
	  if (c_rem == unmapped)
	    {
	      /* removed in local, removed in remote */
	    }
	  else if (c_rem == mapped)
	    {
	      /* removed in local, exists in remote */
	      d_anc = merge_map_get_delta (map, src_ancestor);
	      d_rem = merge_map_get_delta (map, src_remote);

	      anc = (org_heading *) merge_delta_get_elt (d_anc);
	      rem = (org_heading *) merge_delta_get_elt (d_rem);

	      if (is_updated (rem, anc))
		{
		  doc_stream_puts (">>>>> LOCAL\nDELETED\n=====\n", out);
		  org_heading_print_op ((doc_elt *)rem, NULL, out);
		  doc_stream_puts ("<<<<< REMOTE\n", out);
		}
	    }
	}
      else if (c_loc == mapped)
	{
	  if (c_rem == unmapped)
	    {
	      /* exists in local, removed in remote */
	      d_anc = merge_map_get_delta (map, src_ancestor);
	      d_loc = merge_map_get_delta (map, src_local);

	      anc = (org_heading *) merge_delta_get_elt (d_anc);
	      loc = (org_heading *) merge_delta_get_elt (d_loc);

	      if (is_updated (loc, anc))
		{
		  doc_stream_puts (">>>>> LOCAL\n", out);
		  org_heading_print_op ((doc_elt *)loc, NULL, out);
		  doc_stream_puts ("\n=====\nDELETED\n<<<<< REMOTE\n", out);
		}
	    }
	  else if (c_rem == mapped)
	    {
	      /* exists in all 3 */
	      d_anc = merge_map_get_delta (map, src_ancestor);
	      d_loc = merge_map_get_delta (map, src_local);
	      d_rem = merge_map_get_delta (map, src_remote);

	      anc = (org_heading *) merge_delta_get_elt (d_anc);
	      loc = (org_heading *) merge_delta_get_elt (d_loc);
	      rem = (org_heading *) merge_delta_get_elt (d_rem);

	      bool rem_updated = is_updated (rem, anc);
	      bool loc_updated = is_updated (loc, anc);

	      if (rem_updated)
		{
		  if (loc_updated)
		    {
		      doc_stream_puts (">>>>> LOCAL\n", out);
		      org_heading_print_op ((doc_elt *)loc, NULL, out);
		      doc_stream_puts ("=====\n", out);
		      org_heading_print_op ((doc_elt *)rem, NULL, out);
		      doc_stream_puts ("<<<<< REMOTE\n", out);
		    }
		  else
		    {
		      org_heading_print_op ((doc_elt *)rem, NULL, out);
		    }
		}
	      else
		{
		  org_heading_print_op ((doc_elt *)loc, NULL, out);
		}
	    }
	}
    }
  else
    {
      if (c_loc == mapped)
	{
	  d_loc = merge_map_get_delta (map, src_local);
	  loc = (org_heading *) merge_delta_get_elt (d_loc);
	  org_heading_print_op ((doc_elt *)loc, NULL, out);
	}
      else if (c_rem == mapped)
	{
	  d_rem = merge_map_get_delta (map, src_remote);
	  rem = (org_heading *) merge_delta_get_elt (d_rem);
	  org_heading_print_op ((doc_elt *)rem, NULL, out);
	}
    }

  ctxt->depth++;
  return;
}

static int
org_heading_is_related_op (doc_elt *local, doc_src sl, doc_elt *remote, doc_src sr, void *context)
{

  org_heading *hl = (org_heading *) local;
  org_heading *hr = (org_heading *) remote;

  return !(strcmp (hl->heading_text, hr->heading_text));
}

/* Getters and Setters */
int
org_heading_get_level (org_heading *self)
{
  return self->level;
}

void
org_heading_set_level (org_heading *self, int level)
{
  self->level = level;
  return;
}

char *
org_heading_get_text (org_heading *self)
{
  return self->heading_text;
}

void
org_heading_set_text (org_heading *self, char *heading_text)
{
  self->heading_text = heading_text;
  return;
}


static doc_elt_compare_result
org_heading_compare_op (doc_elt *a, doc_src a_src, doc_elt *b, doc_src b_src)
{
  /**
   * @todo make this function compare no identifiable parts of the element
   * this function just returns if the two texts are the same or not
   */

  org_heading *hl = (org_heading *) a;
  org_heading *hr = (org_heading *) b;

  if (!(strcmp (hl->heading_text, hr->heading_text)))
    {
      return elt_compare_same;
    }

  return elt_compare_different;
}
