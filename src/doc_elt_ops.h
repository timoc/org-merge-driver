/**
 * @file doc_elt_ops.h
 */

#ifndef DOC_ELT_OPS
#define DOC_ELT_OPS

#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "doc_stream.h"
#include "doc_elt.h"
#include "doc_ref.h"

typedef struct doc_elt doc_elt;
typedef struct print_ctxt print_ctxt;

typedef enum doc_elt_compare_result
  {
    elt_compare_different,
    elt_compare_same = 0,
    elt_compare_a_updated,
    elt_compare_b_updated
  } doc_elt_compare_result;

/* Printing */
typedef void (doc_elt_ops_print) (doc_ref*, print_ctxt *, doc_stream *);

/* comparing */
typedef int (doc_elt_ops_compare)(doc_elt *, doc_src, doc_elt*, doc_src);
typedef bool (doc_elt_ops_isrelated) (doc_elt *, doc_src, doc_elt *, doc_src, void *);

/* merging */
typedef void (doc_elt_ops_merge) (doc_elt *, doc_elt *, doc_src);
typedef bool (doc_elt_ops_isupdated) (doc_ref *);
typedef void (doc_elt_ops_unmapped) (doc_elt*);

typedef struct doc_elt_ops
{
  /* printing */
  doc_elt_ops_print     *print;         /*< print an element */
  /* comparing */
  doc_elt_ops_compare   *compare;       /*< see if element is updated */
  doc_elt_ops_isrelated *isrelated;     /*< if elements can be mapped */
  /* merging */
  doc_elt_ops_merge     *merge;         /*< merge an element and all its children */
  doc_elt_ops_isupdated *isupdated;     /*< check if an element (or children) is updated */
  doc_elt_ops_unmapped  *mark_unmapped; /*< notify an element it is not mapped */
} doc_elt_ops;

static inline doc_elt_ops *
doc_elt_ops_create_empty ()
{
  return malloc (sizeof (doc_elt_ops));
}

static inline void
doc_elt_ops_free (doc_elt_ops *ops)
{
  assert (ops != NULL);
  free (ops);
}

static inline doc_elt_ops_print*
doc_elt_ops_get_print (doc_elt_ops *ops)
{
  assert (ops != NULL);
  return ops->print;
}

static inline void
doc_elt_ops_set_print (doc_elt_ops *ops, doc_elt_ops_print print)
{
  assert (ops != NULL);
  ops->print = print;
}

static inline doc_elt_ops_compare*
doc_elt_ops_get_compare (doc_elt_ops *ops)
{
  assert (ops != NULL);
  return ops->compare;
}

static inline void
doc_elt_ops_set_compare (doc_elt_ops *ops, doc_elt_ops_compare compare)
{
  assert (ops != NULL);
  ops->compare = compare;
  return;
}

static inline doc_elt_ops_isrelated*
doc_elt_ops_get_isrelated (doc_elt_ops *ops)
{
  assert (ops != NULL);
  return ops->isrelated;
}

static inline void
doc_elt_ops_set_isrelated (doc_elt_ops *ops, doc_elt_ops_isrelated isrelated)
{
  assert (ops != NULL);
  ops->isrelated = isrelated;
  return;
}

static inline doc_elt_ops_merge*
doc_elt_ops_get_merge (doc_elt_ops *ops)
{
  assert (ops != NULL);
  return ops->merge;
}

static inline void
doc_elt_ops_set_merge (doc_elt_ops *ops, doc_elt_ops_merge merge)
{
  assert (ops != NULL);
  ops->merge = merge;
  return;
}

static inline doc_elt_ops_isupdated*
doc_elt_ops_get_isupdated (doc_elt_ops *ops)
{
  assert (ops != NULL);
  return ops->isupdated;
}

static inline void
doc_elt_ops_set_isupdated (doc_elt_ops *ops, doc_elt_ops_isupdated isupdated)
{
  assert (ops != NULL);
  ops->isupdated = isupdated;
  return;
}

static inline doc_elt_ops_unmapped*
doc_elt_ops_get_unmapped (doc_elt_ops *ops)
{
  assert (ops != NULL);
  return ops->mark_unmapped;
}

static inline void
doc_elt_ops_set_unmapped (doc_elt_ops *ops, doc_elt_ops_unmapped unmapped)
{
  assert (ops != NULL);
  ops->mark_unmapped = unmapped;
  return;
}
#endif
