/**
 * @file doc_elt_ops.h
 */

#ifndef DOC_ELT_OPS
#define DOC_ELT_OPS

#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "doc_stream.h"
#include "merge_delta.h"
#include "merge_print_ctxt.h"

struct doc_elt;
typedef struct doc_elt doc_elt;

typedef enum doc_elt_compare_result
  {
    elt_compare_different,
    elt_compare_same,
    elt_compare_a_updated,
    elt_compare_b_updated
  } doc_elt_compare_result;

typedef void(* doc_elt_ops_print)( doc_elt *, void *, doc_stream *);
typedef void(* doc_elt_ops_merge_print)(merge_delta *, merge_print_ctxt *, doc_stream *);
typedef doc_elt_compare_result (* doc_elt_ops_compare)(doc_elt *, doc_src, doc_elt *, doc_src);
typedef int (* doc_elt_ops_is_related)(doc_elt *, doc_src, doc_elt *, doc_src, void *);

typedef struct doc_elt_ops
{
  doc_elt_ops_print       print;
  doc_elt_ops_merge_print merge_print;
  doc_elt_ops_compare     compare;
  doc_elt_ops_is_related  is_related;
} doc_elt_ops;

static inline doc_elt_ops *
doc_elt_ops_create_empty ()
{
  return malloc (sizeof (doc_elt_ops));
}

static inline void
doc_elt_ops_free (doc_elt_ops *ops)
{
  free (ops);
}

static inline doc_elt_ops_print
doc_elt_ops_get_print (doc_elt_ops *ops)
{
  assert (ops->print);
  return ops->print;
}

static inline void
doc_elt_ops_set_print (doc_elt_ops *ops, doc_elt_ops_print print)
{
  assert (ops->print);
  ops->print = print;
}

static inline doc_elt_ops_merge_print
doc_elt_ops_get_merge_print (doc_elt_ops *ops)
{
  assert (ops->merge_print);
  return ops->merge_print;
}

static inline void
doc_elt_ops_set_merge_print (doc_elt_ops *ops,
			     doc_elt_ops_merge_print merge_print)
{
  assert (ops->merge_print);
  ops->merge_print = merge_print;
}


static inline doc_elt_ops_compare
doc_elt_ops_get_compare (doc_elt_ops *ops)
{
  assert (ops->compare);
  return ops->compare;
}


static inline void
doc_elt_ops_set_compare (doc_elt_ops *ops, doc_elt_ops_compare compare)
{
  assert (ops->compare);
  ops->compare = compare;
}

static inline doc_elt_ops_is_related
doc_elt_ops_get_is_related (doc_elt_ops *ops)
{
  assert (ops->is_related);
  return ops->is_related;
}

static inline void
doc_elt_ops_set_is_related (doc_elt_ops *ops, doc_elt_ops_is_related is_related)
{
  assert (ops->is_related);
  ops->is_related = is_related;
}

#endif
