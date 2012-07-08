/**
 * @file doc_elt.h
 */
#ifndef DOC_ELT_H
#define DOC_ELT_H

#include <stdlib.h>
#include <stdbool.h>
#include "doc_elt_ops.h"
#include "doc_stream.h"
#include "doc_tree.h"
#include "merge_delta.h"

typedef struct doc_elt
{
  doc_elt_ops *ops;
} doc_elt;

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

/**
 * @brief print an org_elment to file.
 * @parem self The element to print.
 * @param file The FILE stream to print the element to.
 */
static inline void
doc_elt_print (doc_elt *elt, void * context, doc_stream *out)
{
  doc_elt_ops_get_print (doc_elt_get_ops (elt))(elt, context, out);
}

/**
 * @brief print a merged org_element with conflict markers.
 * @param local The locas version of a file.
 * @param remote The remote version of a file.
 * @param file The file stream to print to.
 */
static inline void
doc_elt_merge_print (doc_elt *elt, merge_delta *delta, void * context, doc_stream *out)
{
  doc_elt_ops_get_merge_print (doc_elt_get_ops (elt)) (delta, context, out);
}

/**
 * @brief Compare two org_elements.
 * @self Compare this element. Uses this elements operations.
 * @other_erement The element to compare with.
 *
 * two org_elements, returning TRUE if they match each other by some
 * distiguishing factor, false otherwise.  It is okay to compare two
 * elements with different operations; they can never be equal and
 * doc_elt_compare will always return false.
 */
static inline bool
doc_elt_compare (doc_elt *elt_a, doc_src s1, doc_elt *elt_b, doc_src s2, void *context)
{
  bool status = false;
  doc_elt_ops *ops = doc_elt_get_ops (elt_a);
  if (ops == doc_elt_get_ops (elt_b))
    {
      status = doc_elt_ops_get_compare (ops)(elt_a, s1, elt_b, s2, context);
    }
  return status;
}

#endif
