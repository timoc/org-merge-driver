#ifndef MERGE_PRINT_H
#define MERGE_PRINT_H

#include "doc_stream.h"
#include "merge_tree.h"

struct merge_print_ctxt;
typedef struct merge_print_ctxt merge_print_ctxt;

void merge_print (merge_tree *tree, const merge_print_ctxt *ctxt, doc_stream *out);

#endif
