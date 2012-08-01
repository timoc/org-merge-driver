/**
 * @file doc_elt.h
 */
#ifndef DOC_ELT_H
#define DOC_ELT_H

#include <stdlib.h>
#include <stdbool.h>
#include "debug.h"

#include "doc_elt_ops.h"
#include "doc_stream.h"

/* search merger type, from smerger.h */
typedef struct smerger smerger;

enum doc_type {
  ORG_DOCUMENT,
  ORG_HEADING,
  ORG_TEXT
};

typedef struct doc_elt
{
  doc_elt_ops *ops;
  size_t type;
} doc_elt;

typedef struct merge_ctxt
{
  smerger *global_smerger;
} merge_ctxt;

typedef struct doc_key
{
  size_t length;
  char *string;
} doc_key;

static inline doc_elt * doc_elt_create_empty ();

static inline void doc_elt_free (doc_elt *elt);

static inline void doc_elt_set_ops (doc_elt *elt, doc_elt_ops *ops);

/**
 * @brief Sot the operations structure of an element
 */
static inline doc_elt_ops *
doc_elt_get_ops (doc_elt *elt);

/**
 * @brief Get the type associated with an element
 */
static inline size_t
doc_elt_get_type (doc_elt *elt_a);

/**
 * @brief Set the type associated with an element
 */
static inline void
doc_elt_set_type (doc_elt *elt_a, size_t type);

/**
 * @brief print a merged org_element with conflict markers.
 * @param local The locas version of a file.
 * @param remote The remote version of a file.
 * @param file The file stream to print to.
 */
static inline void
doc_elt_print (doc_ref* ref, print_ctxt *context, doc_stream *out);

/**
 * @brief Compare two org_elements.
 * @self Compare this element. Uses this elements operations.
 * @other_element The element to compare with.
 *
 * two org_elements, returning TRUE if they match each other by some
 * distiguishing factor, false otherwise.  It is okay to compare two
 * elements with different operations; they can never be considered
 * equal and doc_elt_compare will always return false.
 */
static inline bool
doc_elt_isrelated (doc_ref *ref_a, doc_ref *ref_b, void *context);

static inline bool
doc_elt_isupdated (doc_ref *elt_a);

/**
 * @brief Compare two org_elements.
 * @self Compare this element. Uses this elements operations.
 * @other_element The element to compare with.
 */
static inline int
doc_elt_compare (doc_elt *elt_a, doc_src s1, doc_elt *elt_b, doc_src s2);

/**
 * @brief Merge one element into the next
 * This function permanently changes the first element
 */
static void
doc_elt_merge (doc_ref *ref_a, doc_ref *ref_b, merge_ctxt *ctxt);

static doc_key *
doc_elt_get_key (doc_elt *elt);

static void
doc_elt_note_delete (doc_ref *ref, merge_ctxt *ctxt);

static void
doc_elt_note_insert (doc_ref *ref, merge_ctxt *ctxt);

/********************
 * Implementations
 ********************/

static inline doc_elt *
doc_elt_create_empty ()
{
  malloc (sizeof (doc_elt));
}

static inline void
doc_elt_free (doc_elt *elt)
{
  free (elt);
}

static inline void
doc_elt_set_ops (doc_elt *elt, doc_elt_ops *ops)
{
  elt->ops = ops;
}

static inline doc_elt_ops *
doc_elt_get_ops (doc_elt *elt)
{
  return elt->ops;
}

static inline void
doc_elt_print (doc_ref* ref, print_ctxt *context, doc_stream *out)
{
  doc_elt *elt = doc_ref_get_elt (ref);
  doc_elt_ops_get_print (doc_elt_get_ops (elt)) (ref, context, out);
}

static inline bool
doc_elt_isrelated (doc_ref *ref_a, doc_ref *ref_b, void *context)
{
  bool status = false;
  doc_elt *elt_a = doc_ref_get_elt (ref_a);
  doc_elt_ops *ops = doc_elt_get_ops (elt_a);

  status = doc_elt_ops_get_isrelated (ops) (ref_a, ref_b, context);

  return status;
}

static inline bool
doc_elt_isupdated (doc_ref *ref)
{
  bool status = false;
  doc_elt *elt_a = doc_ref_get_elt (ref);
  doc_elt_ops *ops = doc_elt_get_ops (elt_a);
  status = doc_elt_ops_get_isupdated (ops)(ref);
  return status;
}

static inline int
doc_elt_compare (doc_elt *elt_a, doc_src s1, doc_elt *elt_b, doc_src s2)
{
  int status = 0;
  doc_elt_ops *ops = doc_elt_get_ops (elt_a);
  if (ops == doc_elt_get_ops (elt_b))
    {
      status = doc_elt_ops_get_compare (ops)(elt_a, s1, elt_b, s2);
      debug_msg (DOC_ELT, 3, "ops: %d", status);
    }
  return status;
}

static void
doc_elt_merge (doc_ref *ref_a, doc_ref *ref_b, merge_ctxt *ctxt)
{
  doc_elt *elt = doc_ref_get_elt (ref_a);
  doc_elt_ops_get_merge (doc_elt_get_ops (elt)) (ref_a, ref_b, ctxt);
  return;
}

static inline size_t
doc_elt_get_type (doc_elt *elt_a)
{
  assert (elt_a);
  return elt_a->type;
}

static inline void
doc_elt_set_type (doc_elt *elt_a, size_t type)
{
  elt_a->type = type;
}

static doc_key *
doc_elt_get_key (doc_elt *elt)
{
  return doc_elt_ops_get_get_key (doc_elt_get_ops (elt)) (elt);
}

static void
doc_elt_note_delete (doc_ref *ref, merge_ctxt *ctxt)
{
  doc_elt *elt = doc_ref_get_elt (ref);
  doc_elt_ops_get_note_delete (doc_elt_get_ops (elt)) (ref, ctxt);
  return;
}

static void
doc_elt_note_insert (doc_ref *ref, merge_ctxt *ctxt)
{
  doc_elt *elt = doc_ref_get_elt (ref);
  doc_elt_ops_get_note_insert (doc_elt_get_ops (elt)) (ref, ctxt);
  return;
}

static bool
doc_key_eql (doc_key *a, doc_key *b)
{
  int length = (a->length < b->length) ? a->length : b->length;
  bool eql = false;
  if (a->length == b->length)
    {
      eql = (strncmp(a->string, b->string, length) == 0);
    }
  return eql;
}

#endif /* DOC_ELT_H */
