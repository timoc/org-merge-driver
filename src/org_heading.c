/**
 * @file org_heading.c
 */

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "doc_tree.h"
#include "doc_tree_map.h"
#include "org_heading.h"

/* Forward Declarations */
struct org_heading_ops;
static void org_heading_print_op (doc_elt *self, doc_stream *out);
static void org_heading_print_merge_op (doc_elt *local, doc_tree_delta *delta, doc_stream *out);
static bool org_heading_compare_op (doc_elt *elt_a, doc_elt *elt_b);

/* Declaration of org_element operations table */
static struct doc_elt_ops org_heading_ops = {
  .print        = org_heading_print_op,
  .print_merge  = org_heading_print_merge_op,
  .compare      = org_heading_compare_op
};

/* org_element operations definitions */

/**
 * @brief call org_heading_print on a doc_elt.
 */
static void
org_heading_print_op (doc_elt *self, doc_stream *out)
{
  assert (self != NULL);
  assert (self->ops == &org_heading_ops);
  org_heading_print ((org_heading *)self, out);
  return;
}

/**
 * @brief call org_heading_print_merge
 */
static void
org_heading_print_merge_op (doc_elt *self, doc_tree_delta *delta, doc_stream *out)

{
  assert (self != NULL);
  assert (delta != NULL);
  assert ( self->ops == &org_heading_ops);
  org_heading_print_merge ((org_heading *)self, delta, out);
  return;
}

static bool
org_heading_compare_op (doc_elt *elt_a, doc_elt *elt_b)
{
  assert (elt_a != NULL && elt_b != NULL);
  assert (elt_a->ops == &org_heading_ops);
  assert (elt_b->ops == &org_heading_ops);
  assert (elt_a->ops == elt_b->ops);

  return org_heading_compare ((org_heading *)elt_a, (org_heading *)elt_b);
}

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
  doc_elt_set_ops(&(h->elt), &org_heading_ops);
  return h;
}

void
org_heading_free (org_heading *self)
{
  free (self);
}

/* Utility functions */
void 
org_heading_print (org_heading *self, doc_stream *out)
{
  int i = 0;
  int level = self->level;
  for (i = 0; i < level; i++)
    {
      doc_stream_putc ('*', out);
    }
  doc_stream_putc (' ', out);
  doc_stream_puts (self->heading_text, out);
  doc_stream_putc ('\n', out);
  return;
}

void
org_heading_print_merge (org_heading *self, doc_tree_delta *delta, doc_stream *out)
{
  /* */
  doc_tree_map *map = doc_tree_delta_get_map (delta);

  doc_tree_node *a_node = doc_tree_map_get_ancestor (map);
  doc_tree_node *l_node = doc_tree_map_get_local    (map);
  doc_tree_node *r_node = doc_tree_map_get_remote   (map);
  
  org_heading *ancestor = (org_heading *)doc_tree_node_get_elt (a_node);
  org_heading *local =    (org_heading *)doc_tree_node_get_elt (l_node);
  org_heading *remote =   (org_heading *)doc_tree_node_get_elt (r_node);

  /* Heading Level:
   * Use self->heading_level to print, regardless of file origin */
  inline void print_stars (int level, doc_stream *out)
  {
    int i = 0;
    for (i = 0; i < level; i++)
      {
	doc_stream_putc ('*', out);
      }
    doc_stream_putc (' ', out);
  }

  /* Heading Text */
  bool local_text_updated = (0 == strcmp (ancestor->heading_text, local->heading_text));
  bool remote_text_updated = (0 == strcmp (ancestor->heading_text, remote->heading_text));

  if (local_text_updated && remote_text_updated)
    {
      /* conflict */
      doc_stream_puts (">>>> local\n", out);
      print_stars (self->level, out);
      doc_stream_puts (local->heading_text, out);
      doc_stream_puts ("\n====\n", out);
      print_stars (self->level, out);
      doc_stream_puts (remote->heading_text, out);
      doc_stream_puts ("\n<<<< remote\n", out);
    }
  else 
    {
      /* only one heading text was updated.  Print it at level self->level */
      /* Self indicates the node */
      print_stars (self->level, out);

      if (local_text_updated)
	{
	  doc_stream_puts (self->heading_text, out);
	}
      else if (remote_text_updated)
	{
	  doc_stream_puts (remote->heading_text, out);
	}
      else
	{
	  doc_stream_puts (self->heading_text, out);
	}

      doc_stream_putc ('\n', out);
    }
}

bool
org_heading_compare (org_heading *heading_a, org_heading *heading_b)
{
  bool a = !(strcmp (heading_a->heading_text, heading_b->heading_text));
  bool b = (heading_a->level == heading_b->level);
  return (a && b);
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
