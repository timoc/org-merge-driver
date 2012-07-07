/**
 * @file doc_elt_ops.h
 */

#ifndef DOC_ELT_OPS
#define DOC_ELT_OPS

#include <stdlib.h>
#include <stdbool.h>
#include "doc_stream.h"
#include "merge_delta.h"

struct doc_elt;
typedef struct doc_elt doc_elt;

typedef void(* doc_elt_ops_print)( doc_elt *, void *, doc_stream *);
typedef void(* doc_elt_ops_print_merge)(doc_elt *, merge_delta *, void *, doc_stream *);
typedef bool(* doc_elt_ops_compare)(doc_elt *, doc_elt *, void *);

typedef struct doc_elt_ops
{
  doc_elt_ops_print       print;
  doc_elt_ops_print_merge print_merge;
  doc_elt_ops_compare     compare;
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
  return ops->print;
}

static inline void
doc_elt_ops_set_print (doc_elt_ops *ops, doc_elt_ops_print print)
{
  ops->print = print;
}

static inline doc_elt_ops_print_merge
doc_elt_ops_get_print_merge (doc_elt_ops *ops)
{
  return ops->print_merge;
}

static inline void
doc_elt_ops_set_print_merge (doc_elt_ops *ops,
			     doc_elt_ops_print_merge print_merge)
{
  ops->print_merge = print_merge;
}

static inline doc_elt_ops_compare
doc_elt_ops_get_compare (doc_elt_ops *ops)
{
  return ops->compare;
}

static inline void
doc_elt_ops_set_compare (doc_elt_ops *ops, doc_elt_ops_compare compare)
{
  ops->compare = compare;
}

#endif
