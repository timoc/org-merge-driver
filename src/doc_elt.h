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

typedef struct doc_elt
{
  doc_elt_ops *ops;
} doc_elt;

static inline doc_elt *
doc_elt_create_empty ();

static inline void
doc_elt_free (doc_elt *elt);

static inline void
doc_elt_set_ops (doc_elt *elt, doc_elt_ops *ops);

/**
 * @brief Sot the operations structure of an element
 */
static inline doc_elt_ops *
doc_elt_get_ops (doc_elt *elt);

/**
 * @brief print a merged org_element with conflict markers.
 * @param local The locas version of a file.
 * @param remote The remote version of a file.
 * @param file The file stream to print to.
 */
static inline void
doc_elt_print (doc_elt *elt, doc_ref* ref, print_ctxt *context, doc_stream *out);

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
doc_elt_isrelated (doc_elt *elt_a, doc_src s1, doc_elt *elt_b, doc_src s2, void *context);

static inline bool
doc_elt_isupdated (doc_ref *elt_a);

/**
 * @brief Compare two org_elements.
 * @self Compare this element. Uses this elements operations.
 * @other_element The element to compare with.
 */
static inline doc_elt_compare_result
doc_elt_compare (doc_elt *elt_a, doc_src s1, doc_elt *elt_b, doc_src s2);

/**
 * @brief Merge one element into the next
 * This function permanently changes the first element
 */
static void
doc_elt_merge (doc_elt *ancestor, doc_elt *descendent, doc_src src);

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
doc_elt_print (doc_elt *elt, doc_ref* ref, print_ctxt *context, doc_stream *out)
{
  doc_elt_ops_get_print (doc_elt_get_ops (elt)) (ref, context, out);
}

static inline bool
doc_elt_isrelated (doc_elt *elt_a, doc_src s1, doc_elt *elt_b, doc_src s2, void *context)
{
  bool status = false;
  doc_elt_ops *ops = doc_elt_get_ops (elt_a);
  if (ops == doc_elt_get_ops (elt_b))
    {
      status = doc_elt_ops_get_isrelated (ops)(elt_a, s1, elt_b, s2, context);
    }
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

static inline doc_elt_compare_result
doc_elt_compare (doc_elt *elt_a, doc_src s1, doc_elt *elt_b, doc_src s2)
{
  doc_elt_compare_result status = elt_compare_different;
  doc_elt_ops *ops = doc_elt_get_ops (elt_a);
  if (ops == doc_elt_get_ops (elt_b))
    {
      status = doc_elt_ops_get_compare (ops)(elt_a, s1, elt_b, s2);
      debug_msg (DOC_ELT, 3, "ops: %d", status);
    }
  return status;
}

static void
doc_elt_merge (doc_elt *ancestor, doc_elt *descendent, doc_src src)
{
  doc_elt_ops_get_merge (doc_elt_get_ops (ancestor)) (ancestor, descendent, src);
  return;
}

#endif
