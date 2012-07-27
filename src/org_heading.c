/**
 * @file org_heading.c
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "debug.h"

#include "config.h"
#include "gl_array_list.h"
#include "gl_list.h"

#include "print.h"

#include "doc_elt.h"
#include "doc_elt_ops.h"
#include "doc_elt_util.h"
#include "doc_ref.h"
#include "org_heading.h"
#include "org_text.h"

/* Forward Declarations */
/* org_heading_data */
typedef struct org_heading_data org_heading_data;
static void org_heading_data_free (org_heading_data *self);
static org_heading_data *org_heading_data_create_empty ();
static bool org_heading_content_isupdated (org_heading *heading, size_t data_index);
static bool org_heading_subelts_isupdated (org_heading *heading);

static void print_LU_RU (doc_ref *ref, print_ctxt *ctxt, doc_stream *out);
static void print_LU_RD (doc_ref *ref, print_ctxt *ctxt, doc_stream *out);
static void print_LD_RU (doc_ref *ref, print_ctxt *ctxt, doc_stream *out);
static void print_LD_RD (doc_ref *ref, print_ctxt *ctxt, doc_stream *out);
static void print_LI_RI (doc_ref *ref, print_ctxt *ctxt, doc_stream *out);
static void print_LI_RX (doc_ref *ref, print_ctxt *ctxt, doc_stream *out);
static void print_LX_RI (doc_ref *ref, print_ctxt *ctxt, doc_stream *out);
static void print_LX_RX (doc_ref *ref, print_ctxt *ctxt, doc_stream *out);

static void print_LR (doc_ref *ref, print_ctxt *ctxt, doc_stream *out);
static void print_L (doc_ref *ref, print_ctxt *ctxt, doc_stream *out);
static void print_R (doc_ref *ref, print_ctxt *ctxt, doc_stream *out);

static void print (org_heading *h, size_t index, print_ctxt *ctxt, doc_stream *out);
static void print_subelts (doc_ref *ref, print_ctxt *ctxt, doc_stream *out);

/* merging helpers */
static void org_movement_merge_print (doc_ref *ref, print_ctxt *ctxt, doc_stream *out);

/* property */
typedef struct property property;

/* Declaration of org_element operations table */
static doc_elt_ops_print     org_heading_print_op;
static doc_elt_ops_isrelated org_heading_isrelated_op;
static doc_elt_ops_compare   org_heading_compare_op;
static doc_elt_ops_merge     org_heading_merge_op;
static doc_elt_ops_isupdated org_heading_isupdated_op;
static doc_elt_ops_unmapped  org_heading_unmapped_op;

doc_elt_ops org_heading_ops = {
/* printing */
.print         = &org_heading_print_op,

 /* comparing */
   .isrelated     = &org_heading_isrelated_op,
   .compare       = &org_heading_compare_op,

 /* merging */
   .merge         = &org_heading_merge_op,
   .isupdated     = &org_heading_isupdated_op,
   .mark_unmapped = &org_heading_unmapped_op,
};

/* Core org_heading struct */
typedef struct org_heading_data
{
  int       level;        /*< The heading level, number of stars. */
  substr    text;         /*< The complete heading text. No stars. */
  substr    todo;         /*< The todo state substring */
  substr    pre_cookie;   /*< Cookies placed before heading text */
  substr    body_text;    /*< The basic heading text */
  substr    post_cookie;  /*< Cookies placed before heading text */
  property *uid;
  gl_list_t properties;
  gl_list_t tags;
} org_heading_data;

/* merged org_heading struct */
typedef struct org_heading
{
  doc_elt           elt;          /*< The element interface. */
  org_heading_data* data[3];      /*< The data for each elt version */
  bool              isupdated[3]; /*< Indicates if a corresponding data entry is updated. */
  bool              moved[2];     /*< Indicates if a corresponding vesion was moved */
  gl_list_t         subtext;      /*< A list of children text elements. */
  gl_list_t         subheadings;  /*< A list of children heading elements. */
} org_heading;

/**
 * Property
 */
typedef struct property
{
  substr string; /*< A string representation of the entire line. */
  substr key;    /*< The property key, a substring. */
  substr value;  /*< The property value, a substring. */
} property;

/* Constructor, Destructor */
org_heading *
org_heading_create_empty (doc_elt_ops *elt_ops)
{
  org_heading *h = calloc (1, sizeof (org_heading));
  doc_elt_set_ops (&(h->elt), elt_ops);

  h->subheadings = gl_list_nx_create_empty (GL_ARRAY_LIST, NULL, NULL, NULL, true);
  h->subtext     = gl_list_nx_create_empty (GL_ARRAY_LIST, NULL, NULL, NULL, true);
  return h;
}

void
org_heading_free (org_heading *self)
{
  /**
   * @todo Implement org_heading_free.
   */
  free (self);
}

void
org_heading_initversion (org_heading *heading, doc_src src)
{
  heading->data[srctoindex(src)] = org_heading_data_create_empty();
  return;
}

bool
org_heading_containsversion (org_heading *heading, doc_src src)
{
  return (heading->data[srctoindex(src)] != NULL);
}

static org_heading_data *
org_heading_data_create_empty ()
{
  org_heading_data *h = calloc (1, sizeof (org_heading_data));
  h->properties =  gl_list_nx_create_empty (GL_ARRAY_LIST, NULL, NULL, NULL, true);
  h->tags       =  gl_list_nx_create_empty (GL_ARRAY_LIST, NULL, NULL, NULL, true);
  return h;
}

static void
org_heading_data_free (org_heading_data *self)
{
  free (self);
}

/* Adding sub elements */
void
org_heading_add_subtext_last (org_heading *heading, doc_src src, org_text *text)
{
  /* wrap the element in a doc_ref, and add it to the list */
  doc_ref *ref = doc_ref_create_empty ();
  doc_ref_set_src (ref, src);
  doc_ref_set_elt (ref, (doc_elt *) text);
  gl_list_nx_add_last (heading->subtext, ref);
  return;
}

void
org_heading_add_subheading_last (org_heading *heading, doc_src src, org_heading *subheading)
{
  /* wrap the element in a doc_ref, and add it to the list */
  doc_ref *ref = doc_ref_create_empty ();
  doc_ref_add_src (ref, src);
  doc_ref_set_elt (ref, (doc_elt *) subheading);
  gl_list_nx_add_last (heading->subheadings, ref);
  return;
}

/* Getters and Setters */
gl_list_t
org_heading_get_subtext (org_heading *self)
{
  return self->subtext;
}

void
org_heading_set_subtext (org_heading *self, gl_list_t text)
{
  self->subtext = text;
  return;
}

int
org_heading_get_level (org_heading *self, doc_src src)
{
  assert (self != NULL);
  data_index i = srctoindex (src);
  org_heading_data *data = self->data[i];
  int level = 0;
  assert (data != NULL);
  if (data != NULL)
    {
      level = data->level;
    }
  return level;
}

void
org_heading_set_level (org_heading *self, int level, doc_src src)
{
  assert (self != NULL);
  data_index i = srctoindex(src);
  org_heading_data *data = self->data[i];
  assert (data != NULL);
  if (data != NULL)
    {
      data->level = level;
    }
  return;
}

char *
org_heading_get_text (org_heading*self, doc_src src)
{
  assert (self != NULL);
  org_heading_data *data = self->data[srctoindex(src)];
  assert (data != NULL);
  char *text;
  if (data != NULL)
    {
      text = data->text.string;
    }
  return text;
}

size_t
org_heading_get_length (org_heading *heading, doc_src src)
{
  assert (heading != NULL);
  org_heading_data *data = heading->data[srctoindex(src)];
  assert (data != NULL);
  size_t length;
  if (data != NULL)
    {
      length = data->text.length;
    }
  return length;
}

void
org_heading_set_text (org_heading *self, char *string, int length, doc_src src)
{
  /**
   * @todo Have org_heading parse it's string and set up it's substrings.
   */

  assert (self != NULL);
  org_heading_data *data = self->data[srctoindex(src)];
  assert (data != NULL);
  if (data != NULL)
    {
      data->text.string = string;
      data->text.length = length;
      data->body_text.string = string;
      data->body_text.length = length;

      /*
       * For now, have body_text begin at the beginning of the heading
       * text, and go until the first tag, marked by a colon (:).
       * Leave the other subtexts empty.
       */
      int i = 0;
      for (i = 0; i < length; i++)
	{
	  if (string[i] == ':')
	    {
	      data->body_text.length = i - 1;
	      break;
	    }
	}
    }
  return;
}

#if 0

void
org_heading_add_property_empty (char *string, doc_src src)
{
  /* keep a reference to the string, store the value and the key as
   * pointers into the string.
   */

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

#endif

/*
 * doc_elt interface
 */

static bool
org_heading_isrelated_op (doc_elt *a_elt, doc_src a_src, doc_elt *b_elt, doc_src b_src, void *context)
{

  /**
   * @todo Make org_heading_isrelated_op check for matching doc_elt_types.
   */

  org_heading      *a_heading = (org_heading *)a_elt;
  org_heading_data *a_data    = a_heading->data[srctoindex(a_src)];
  org_heading      *b_heading = (org_heading *)b_elt;
  org_heading_data *b_data    = b_heading->data[srctoindex(b_src)];

  /**
   * @todo Make org_heading_isrelated_op check for matching uid.
   */
 
  /**
   * @todo Make org_heading_isrelated_op skip over cookies.
   */

  /* Org_heading will skip over cookies in the body_text when testing
     for is_related.  Cookies are wrapped in square brackets. */
  
  return (substreql(a_data->body_text, b_data->body_text));
}

static int
org_heading_compare_op (doc_elt *a, doc_src a_src, doc_elt *b, doc_src b_src)
{
  /**
   * @todo Implement org_heading compare operator.
   */
  return 0;
}

static void
org_heading_merge_op (doc_elt *a_elt, doc_elt *b_elt, doc_src b_src)
{
  debug_msg (DOC_ELT, 3, "Merging org_heading\n");

  org_heading *a_heading = (org_heading *)a_elt;
  org_heading *b_heading = (org_heading *)b_elt;

  assert (a_heading != NULL);
  assert (b_heading != NULL);
  assert (b_heading->data[srctoindex (b_src)] != NULL);

  a_heading->data[srctoindex (b_src)] = b_heading->data[srctoindex (b_src)];

  debug_msg (DOC_ELT, 5, "Merging heading subtext\n");
  doc_reflist_merge (a_heading->subtext, b_heading->subtext);

  debug_msg (DOC_ELT, 3, "Merging heading subheadings\n");
  doc_reflist_merge (a_heading->subheadings, b_heading->subheadings);

  return;
}

/*
 * Isupdated_op will return true if any version located at the passed
 * ref is updated from the ancestor in a way that might conflict with
 * a parent update.
 *
 * This function is primarily used to test for conflicts with delete
 * operations.  If the parent was deleted, a child update will
 * conflict with the delete at the parent's level. This function
 * counts inserted elements as updates, but does not count deletions.
 */
static bool
org_heading_isupdated_op (doc_ref *ref)
{
  doc_elt *elt = doc_ref_get_elt (ref);
  org_heading *heading = (org_heading *)elt;
  org_heading_data *anc_data;
  org_heading_data *loc_data;
  org_heading_data *rem_data;

  anc_data = heading->data[ANC_INDEX];
  loc_data = heading->data[LOC_INDEX];
  rem_data = heading->data[REM_INDEX];

  bool isupdated = false;

  /* Check each version located at ref for updates.  If a version's
     data is nonexistant, then do not count that as an update.*/
  if ((loc_data != NULL) && doc_ref_contains (ref, LOC_SRC))
    {
      isupdated = isupdated || org_heading_content_isupdated(heading, LOC_INDEX);
    }

  if ((rem_data != NULL) && doc_ref_contains (ref, REM_SRC))
    {
      isupdated = isupdated || org_heading_content_isupdated(heading, REM_INDEX);
    }

  isupdated = isupdated || org_heading_subelts_isupdated (heading);
  return isupdated;
}


static void
org_heading_unmapped_op (doc_elt *elt)
{
  /* mark all the children as unmapped */
  return;
}

/**
 * Compare the data in slot data_index with the ancestor data.  Try to
 * detect if the content has been updated.
 */
static bool
org_heading_content_isupdated (org_heading *heading, size_t data_index)
{
  org_heading_data *anc_data = heading->data[ANC_INDEX];
  org_heading_data *new_data = heading->data[data_index];

  /**
   * @todo Check for add and delete operations.  These operations
   * count as updates according to this function.
   */
 
  bool isupdated = false;

  if (anc_data != NULL)
    {
      if (new_data != NULL)
	{
	  isupdated = (substreql (anc_data->text, new_data->text) == 0);
	}
      else
	{
	}
    }
  else
    {
      if (new_data != NULL)
	{
	  isupdated = true;
	}
      else
	{
	}
    }

  /**
   * @todo check all data for updates, not just the heading line.
   */

  /**
   * @todo Cache the calculated update.  This may require a
   * force_isupdated function.
   */

  return isupdated;
}

static bool
org_heading_subelts_isupdated (org_heading *heading)
{
  /**
   * @todo Cache the calculated isupdated status.
   */

  return
    doc_reflist_isupdated (heading->subheadings) ||
    doc_reflist_isupdated (heading->subtext);
}


/*
 * Printing and Merging.
 */

void
org_heading_print_op (doc_ref *ref, print_ctxt *ctxt, doc_stream *out)
{

  /* Letter | Meaning
   *      U | Updated: An updated element exists in this vesion.
   *      D | Deleted: The element was deleted in this version.
   *      I | Inserted: The element is new, and has no ancestor.
   *      X | Does not exist: The element is unassociated with the version.
   */
  org_heading *heading = (org_heading *)doc_ref_get_elt (ref);
 
  if ( heading->data[ANC_INDEX] != NULL) 
    {
      if (heading->data[LOC_INDEX] != NULL)
        {
          if (heading->data[REM_INDEX] != NULL)
            {
              print_LU_RU (ref, ctxt, out);
            } 
          else 
            {
              print_LU_RD (ref, ctxt, out);
            }
        }
      else
        {
          if (heading->data[REM_INDEX] != NULL)
            {
              print_LD_RU (ref, ctxt, out);
            }
          else
            {
              print_LD_RD (ref, ctxt, out);
            }
        }
    }
  else
    {
      if (heading->data[LOC_INDEX] != NULL)
        {
          if (heading->data[REM_INDEX] != NULL)
            {
              print_LI_RI (ref, ctxt, out);
            } 
          else 
            {
              print_LI_RX (ref, ctxt, out);
            }
        }
      else
        {
          if (heading->data[REM_INDEX] != NULL)
            {
              print_LX_RI (ref, ctxt, out);
            }
          else
            {
              print_LX_RX (ref, ctxt, out);
            }
        }
    }
  return;
}

static void
print_LU_RU (doc_ref *ref, print_ctxt *ctxt, doc_stream *out)
{
  debug_msg (DOC_ELT, 5, "Begin\n");
  org_heading *h = (org_heading *)doc_ref_get_elt (ref);
  bool loc_moved = h->moved[LOC_INDEX];
  bool rem_moved = h->moved[REM_INDEX];
  bool loc_ishere = doc_ref_contains (ref, LOC_SRC);
  bool rem_ishere = doc_ref_contains (ref, REM_SRC);

  if (loc_moved)
    {
      if (rem_moved)
        {
          if (loc_ishere)
            {
              if (rem_ishere)
                {
                  print_LR (ref, ctxt, out);
                }
              else
                {
                  enter_structural_conflict (ctxt, local_side, "Moved\n", out);
                  print_LR (ref, ctxt, out);
                  print_subelts (ref, ctxt, out);
                  enter_structural_conflict (ctxt, remote_side, NULL, out);
                  enter_structural_conflict (ctxt, no_conflict, "Moved\n", out);
                }
            }
          else
            {
              if (rem_ishere)
                {
                  enter_structural_conflict (ctxt, local_side, "Moved\n", out);
                  enter_structural_conflict (ctxt, remote_side, NULL, out);
                  print_LR (ref, ctxt, out);
                  print_subelts (ref, ctxt, out);
                  enter_structural_conflict (ctxt, no_conflict, "Moved\n", out);
                }
              else
                {
                  /* Print Nothing */
                }
            }
        }
      else
        {
          if (loc_ishere)
            {
              print_LR (ref, ctxt, out);
              print_subelts (ref, ctxt, out);
            }
          else
            {
            }
        }
    }
  else
    {
      if (rem_moved)
        {
          if (rem_ishere)
            {
              print_LR (ref, ctxt, out);
              print_subelts (ref, ctxt, out);
            }
          else
            {
            }
        }
      else
        {
          if (loc_ishere && rem_ishere)
            {
              print_LR (ref, ctxt, out);
              print_subelts (ref, ctxt, out);
            }
          else
            {
            }
        }
    }
  return;
}

static inline void
print_LU_RD (doc_ref *ref, print_ctxt *ctxt, doc_stream *out)
{
  debug_msg (DOC_ELT, 5, "Begin\n");
  doc_elt *elt = doc_ref_get_elt (ref);
  org_heading *heading = (org_heading *)elt;
  bool local_ishere = doc_ref_contains (ref, LOC_SRC);

  if (local_ishere)
    {
      if (org_heading_content_isupdated (heading, LOC_INDEX) || 
	  org_heading_subelts_isupdated (heading))
        {
          enter_structural_conflict (ctxt, local_side, NULL, out);
          print_L (ref, ctxt, out);
	  print_subelts (ref, ctxt, out);
          enter_structural_conflict (ctxt, remote_side, NULL, out);
          enter_structural_conflict (ctxt, no_conflict, "Deleted\n", out);
        }
    }
  return;
}

static inline void
print_LD_RU (doc_ref *ref, print_ctxt *ctxt, doc_stream *out)
{ 
  debug_msg (DOC_ELT, 5, "Begin\n");
  doc_elt *elt = doc_ref_get_elt (ref);
  org_heading *heading = (org_heading *)elt;
  bool remote_ishere = doc_ref_contains (ref, REM_SRC);

  debug_msg (DOC_ELT, 5, "remote_ishere = %d\n", remote_ishere);
  debug_msg (DOC_ELT, 5, "doc_ref = %d\n", doc_ref_get_src (ref));
  if (remote_ishere)
    {
      if (org_heading_content_isupdated (heading, REM_INDEX) || 
	  org_heading_subelts_isupdated (heading))
        {
          enter_structural_conflict (ctxt, local_side, "Deleted\n", out);
          enter_structural_conflict (ctxt, remote_side, NULL, out);
          print_R (ref, ctxt, out);
	  print_subelts (ref, ctxt, out);
          enter_structural_conflict (ctxt, no_conflict, NULL, out);
        }
    }
  return;
}

static inline void
print_LD_RD (doc_ref *ref, print_ctxt *ctxt, doc_stream *out)
{
  debug_msg (DOC_ELT, 5, "Begin\n");
  return;
}

static inline void
print_LI_RI (doc_ref *ref, print_ctxt *ctxt, doc_stream *out)
{
  debug_msg (DOC_ELT, 5, "Begin\n");
  bool loc_ishere = doc_ref_contains (ref, LOC_SRC);
  bool rem_ishere = doc_ref_contains (ref, REM_SRC);

  if (loc_ishere && rem_ishere)
    {
      print_LR (ref, ctxt, out); 
      print_subelts (ref, ctxt, out);
    }
  return;
}

static inline void
print_LI_RX (doc_ref *ref, print_ctxt *ctxt, doc_stream *out)
{
  debug_msg (DOC_ELT, 5, "Begin\n");
  bool loc_ishere = doc_ref_contains (ref, LOC_SRC);
  if (loc_ishere)
    {
      print_L (ref, ctxt, out); 
      print_subelts (ref, ctxt, out);
    }
  return;
}

/**
 * The remote version of the heading is inserted.  There is no
 * corresponding local version.  Print the remote version if it is
 * located at the current doc ref.  It should always be located at the
 * doc_ref, because there is no corresponding ancestor.
 */
static inline void
print_LX_RI (doc_ref *ref, print_ctxt *ctxt, doc_stream *out)
{
  debug_msg (DOC_ELT, 5, "Begin\n");
  bool rem_ishere = doc_ref_contains (ref, REM_SRC);
  if (rem_ishere)
    {
      print_R (ref, ctxt, out); 
      print_subelts (ref, ctxt, out);
    }
  return;
}

/**
 * Print the heading where local never existed, and remote never
 * existed.  Should never be called.
 */
static inline void
print_LX_RX (doc_ref *ref, print_ctxt *ctxt, doc_stream *out)
{
  debug_msg (DOC_ELT, 5, "Begin\n");
  return;
}

/**
 * Print the local and remote versions of the heading merged.  Will
 * cause a content conflict if the updates cannot be consolidated.
 */
static void
print_LR (doc_ref *ref, print_ctxt *ctxt, doc_stream *out)
{
  debug_msg (DOC_ELT, 5, "Begin\n");
  org_heading *h = (org_heading *)doc_ref_get_elt (ref);
  bool loc_isupdated = org_heading_content_isupdated (h, LOC_INDEX);
  bool rem_isupdated = org_heading_content_isupdated (h, REM_INDEX);

  /**
   * @todo Properly merge two updated heading elements. For now, just
   * conflict them if they are different.
   */

  if (loc_isupdated)
    {
      if (rem_isupdated)
        {
          enter_content_conflict (ctxt, local_side, "Updated\n", out);
          print (h, LOC_INDEX, ctxt, out);
          enter_content_conflict (ctxt, remote_side, NULL, out);
          print (h, REM_INDEX, ctxt, out);
          enter_content_conflict (ctxt, no_conflict, "Updated\n", out);
        }
      else
        {
          print (h, LOC_INDEX, ctxt, out);
        }
    }
  else
    {
      if (rem_isupdated)
        {
          print (h, REM_INDEX, ctxt, out);
        }
      else
        {
          print (h, ANC_INDEX, ctxt, out);
        }
    } 
  return;                                 
}

/**
 * Print the local version of the heading.
 */
static inline void
print_L (doc_ref *ref, print_ctxt *ctxt, doc_stream *out)
{
  debug_msg (DOC_ELT, 5, "Begin\n");
  org_heading *h  = (org_heading *)doc_ref_get_elt (ref);
  print (h, LOC_INDEX, ctxt, out);
  return;
}

/**
 * Print the remote version of the heading.
 */
static inline void
print_R (doc_ref *ref, print_ctxt *ctxt, doc_stream *out)
{
  debug_msg (DOC_ELT, 5, "Begin\n");
  org_heading *h  = (org_heading *)doc_ref_get_elt (ref);
  print (h, REM_INDEX, ctxt, out);
  return;
}

/**
 * Print the version of the heading corresponding to the index.
 */
static void
print (org_heading *h, size_t index, print_ctxt *ctxt, doc_stream *out)
{
  int i = 0;
  for (i = 0; i < h->data[index]->level; i++)
    {
      doc_stream_putc('*', out);
    }
  doc_stream_putc (' ', out);
  substrprint (h->data[index]->text, out);
  return;
}

/**
 *
 */
static void
print_subelts (doc_ref *ref, print_ctxt *ctxt, doc_stream *out)
{
  /**
   * @todo implement print_subelts
   */
  org_heading *h = (org_heading *)doc_ref_get_elt (ref);
  doc_reflist_print (h->subtext, ctxt, out);
  doc_reflist_print (h->subheadings, ctxt, out);
  return;
}
