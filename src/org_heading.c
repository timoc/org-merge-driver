/**
 * @file org_heading.c
 */

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#include "debug.h"

/* for property lists */
#include "config.h"
#include "gl_array_list.h"
#include "gl_list.h"

#include "merge_map.h"
#include "doc_tree.h"
#include "merge_tree.h"
#include "doc_elt.h"
#include "doc_elt_ops.h"
#include "org_heading.h"
#include "merge_print_ctxt.h"

#include "org_text.h"

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
  char* UID;
  bool have_property_list;
  gl_list_t properties;
  org_text * text;
};

/* Property */
typedef struct property
{
  char *key;   /* the property key */
  size_t key_length;
  char *value; /* the property value */
  size_t value_length;
  char *string;/* string representation of the entire line */
} property;

/* Constructor, Destructor */
org_heading *
org_heading_create_empty ()
{
  org_heading *h = malloc (sizeof (org_heading));
  h->heading_text = NULL;
  doc_elt_set_ops (&(h->elt), &org_heading_ops);
  h->have_property_list = false;
  h->properties =  gl_list_nx_create_empty (GL_ARRAY_LIST, NULL,
					    NULL, NULL, true);
  h->text = org_text_create_empty ();
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

  if (c_anc == mapped)
    {
      if (c_loc == unmapped)
	{
	  if (c_rem == unmapped)
	    {
	      /* removed in local, removed in remote
	       * do not print anything */
	      /* it is impossible that a child was updated */
	      debug_msg (DOC, 3, "Heading double deleted\n");
	    }
	  else if (c_rem == mapped)
	    {
	      debug_msg (DOC, 3, "Heading deleted in local\n");

	      /* removed in local, exists in remote */
	      d_anc = merge_map_get_delta (map, src_ancestor);
	      d_rem = merge_map_get_delta (map, src_remote);

	      anc = (org_heading *) merge_delta_get_elt (d_anc);
	      rem = (org_heading *) merge_delta_get_elt (d_rem);

	      if (merge_delta_get_child_update (delta))
		{
		  debug_msg (DOC, 3, "Heading deleted conflicted\n");
		  mark_structural_conflict (ctxt, out);
		  mark_structural_conflict (ctxt, out);
		  org_heading_print_op ((doc_elt *)rem, NULL, out);
		  org_text *t = org_heading_get_text_elt (rem);
		  doc_elt_print ((doc_elt *)t, NULL, out);
		}
	    }
	}
      else if (c_loc == mapped)
	{
	  if (c_rem == unmapped)
	    {
	      debug_msg (DOC, 3, "Heading deleted in remote\n");
	      /* removed in remote, exists in local */
	      d_anc = merge_map_get_delta (map, src_ancestor);
	      d_loc = merge_map_get_delta (map, src_local);

	      anc = (org_heading *) merge_delta_get_elt (d_anc);
	      loc = (org_heading *) merge_delta_get_elt (d_loc);

	      if (org_heading_compare_op ((doc_elt *)loc, src_local,
					  (doc_elt *)anc, src_ancestor))
		{
		}
	      if (merge_delta_get_child_update (delta))
		{

		  debug_msg (DOC, 3, "Heading deleted conflicted\n");
		  mark_structural_conflict (ctxt, out);
		  org_heading_print_op ((doc_elt *)loc, NULL, out);
		  org_text *t = org_heading_get_text_elt (loc);
		  doc_elt_print ((doc_elt *)t, NULL, out);
		}
	    }
	  else if (c_rem == mapped)
	    {
	      debug_msg (DOC, 3, "Heading exists in 3\n");
	      /* exists in all 3 */
	      d_anc = merge_map_get_delta (map, src_ancestor);
	      d_loc = merge_map_get_delta (map, src_local);
	      d_rem = merge_map_get_delta (map, src_remote);

	      anc = (org_heading *) merge_delta_get_elt (d_anc);
	      loc = (org_heading *) merge_delta_get_elt (d_loc);
	      rem = (org_heading *) merge_delta_get_elt (d_rem);

	      bool rem_updated = org_heading_compare_op ((doc_elt *)rem, src_remote,
							 (doc_elt *)anc, src_ancestor);
	      bool loc_updated = org_heading_compare_op ((doc_elt *)rem, src_remote,
							 (doc_elt *)anc, src_ancestor);

	      if (rem_updated)
		{
		  if (loc_updated)
		    {
		      /**
		       * updated in all files
		       */
		      org_heading_print_op ((doc_elt *)loc, NULL, out);

		      mark_content_conflict (ctxt, out);
		      org_text *t = org_heading_get_text_elt (loc);
		      doc_elt_print ((doc_elt *)t, NULL, out);

		      mark_content_conflict (ctxt, out);
		      t = org_heading_get_text_elt (rem);
		      doc_elt_print ((doc_elt *)t, NULL, out);
		      mark_content_conflict (ctxt, out);
		    }
		  else
		    {
		      /* remote updated the heading */
		      org_heading_print_op ((doc_elt *)rem, NULL, out);
		      org_text *t = org_heading_get_text_elt (rem);
		      doc_elt_print ((doc_elt *)t, NULL, out);
		    }
		}
	      else
		{
		  /* local updated the heading */
		  org_heading_print_op ((doc_elt *)loc, NULL, out);
		  org_text *t = org_heading_get_text_elt (loc);
		  doc_elt_print ((doc_elt *)t, NULL, out);
		}
	    }
	}
    }
  else /* did not exist in the ancestor */
    {
      /**
       * @todo do not assume that a heading when inserted did not
       * have a map in the other file
       */
      if (c_loc == mapped)
	{
	  debug_msg (DOC, 3, "Heading added in local\n");
	  d_loc = merge_map_get_delta (map, src_local);
	  loc = (org_heading *) merge_delta_get_elt (d_loc);
	  org_heading_print_op ((doc_elt *)loc, NULL, out);
	  org_text *t = org_heading_get_text_elt (loc);
	  doc_elt_print ((doc_elt *)t, NULL, out);
	}
      else if (c_rem == mapped)
	{
	  debug_msg (DOC, 3, "Heading added in remote\n");
	  d_rem = merge_map_get_delta (map, src_remote);
	  rem = (org_heading *) merge_delta_get_elt (d_rem);
	  org_heading_print_op ((doc_elt *)rem, NULL, out);
	  org_text *t = org_heading_get_text_elt (rem);
	  doc_elt_print ((doc_elt *)t, NULL, out);
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

void
org_heading_set_have_property_list (org_heading *h, bool have_one)
{
  h->have_property_list = have_one;
}

void
org_heading_add_property_empty (char *string)
{
  /* keep a reference to the string, store the value and the key as
   * pointers into the string */

  property *p = malloc (sizeof (property));
  p->string = string;
  p->key = NULL;
  p->value = NULL;

  bool exit = false;
  size_t offset = 0;
  int state = 0;
  size_t temp = 0;

  /* scan the string for a property and key */
  while (!exit)
    {
      /* if at the end of a string, exit early */
      if (string[offset] == '\n' || string[offset] == '\0')
	{
	  exit = true;
	  break;
	}

      switch (state)
	{
	case 0: /* searching for the start of the key */
	  if (string[offset] == ':')
	    {
	      p->key = string + offset + 1;
	      state = 1;
	    }
	  offset++;
	  break;

	case 1: /* setting the key */
	  if (string[offset] == ':')
	    {
	      p->key_length = temp;
	      state = 2;
	      temp = 0;
	      offset++;
	      break;
	    }
	  temp++;
	  offset++;
	  break;

	case 2: /* eat white spaces before the value */
	  if (string[offset] != ' ')
	    {
	      p->value = string + offset;
	      p->value_length = 1;
	      state = 3;
	      temp = 1;
	      offset++;
	      break;
	    }
	  offset++;
	  break;

	case 3: /* set the value */
	  if (string[offset] != ' ')
	    {
	      /* only grab nonblank space */
	      p->value_length = temp;
	    }
	  offset++;
	  temp++;
	  break;
	}

      /* check to see if the property had the correct form */
      if (DOC_ELT_PRINTLEVEL <= 5)
	{
	  debug_msg (DOC_ELT, 5, "property: \"");
	  fwrite (p->key, sizeof (char), p->key_length, stderr );
	  debug_msg (DOC_ELT, 5, "\" \"");
	  fwrite (p->value, sizeof (char), p->value_length, stderr );
	  debug_msg (DOC_ELT, 5, "\n");
	}
    }
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
  doc_elt_compare_result r = elt_compare_same;
  r = doc_elt_compare ((doc_elt *)org_heading_get_text_elt((org_heading *)a), a_src,
		       (doc_elt *)org_heading_get_text_elt((org_heading *)b), b_src);


  return r;
}

/* Getters and Setters */
org_text *
org_heading_get_text_elt (org_heading *self)
{
  return self->text;
}

void
org_heading_set_text_elt (org_heading *self, org_text * text)
{
  self->text = text;
  return;
}
