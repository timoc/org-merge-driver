#ifndef ORG_TEXT_H
#define ORG_TEXT_H

#include "doc_elt.h"
#include "doc_stream.h"
#include "merge_delta.h"

struct org_text;
typedef struct org_text org_text;

org_text *org_text_create_empty ();
void org_text_free (org_text * self);

/**
 * @brief Append characters from string to org_text.
 * 
 * The first num characters in str are copied out of str into new
 * memory.  After this function is called, it is safe to dispose of
 * str.
 */
void org_text_strncat (org_text *self, char *str, size_t num);

char *org_text_get_text (org_text *self);

size_t org_text_get_text_size (org_text *self);

#endif
