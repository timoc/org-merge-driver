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
#include "doc_tree_delta.h"

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
doc_elt_print (doc_elt *elt, doc_stream *out)
{
  doc_elt_ops_get_print (doc_elt_get_ops (elt))(elt, out);
}

/**
 * @brief print a merged org_element with conflict markers.
 * @param local The locas version of a file.
 * @param remote The remote version of a file.
 * @param file The file stream to print to.
 */
static inline void
doc_elt_print_merge (doc_elt *elt, doc_tree_delta *delta, doc_stream *out)
{
  doc_elt_ops_get_print_merge (doc_elt_get_ops (elt))(elt, delta, out);
}

/**
 * @brief Compare two org_elements.  
 * @self Compare this element. Uses this elements operations.
 * @other_erement The element to compare with.
 *
 * two org_elements, returning TRUE if they match each other by some
 * distiguishing factor, false otherwise.  It is okay to compare to
 * elements with different operations.
 */
static inline bool
doc_elt_compare (doc_elt *elt_a, doc_elt *elt_b)
{
  bool status = false;
  doc_elt_ops *ops = doc_elt_get_ops (elt_a);
  if (ops == doc_elt_get_ops (elt_b))
    {
      status = doc_elt_ops_get_compare (ops)(elt_a, elt_b);
    }
  return status;
}

#endif
