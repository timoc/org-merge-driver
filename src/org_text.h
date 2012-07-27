#ifndef ORG_TEXT_H
#define ORG_TEXT_H

#include "doc_elt.h"
#include "doc_stream.h"
#include "doc_ref.h"

struct org_text;
typedef struct org_text org_text;

/* Constructor, destructor */
org_text *org_text_create_empty (doc_elt_ops *ops);
void org_text_free (org_text * self);

void org_text_initversion (org_text *text, doc_src src);
bool org_text_containsversion (org_text *text, doc_src src);

/* doc_elt_ops implementation */
typedef struct doc_elt_ops doc_elt_ops;
extern doc_elt_ops org_text_ops;

void org_text_set_text (org_text *text, char *string, size_t length, doc_src src);
char *org_text_get_text (org_text *text, doc_src src);
size_t org_text_get_length (org_text *text, doc_src src);

#endif
