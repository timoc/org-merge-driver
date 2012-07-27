/**
 * @file org_heading.h
 */

#ifndef ORG_HEADING_H
#define ORG_HEADING_H

#include "stddef.h"
#include "stdbool.h"

/* org_text forward declaration */
struct org_text;
typedef struct org_text org_text;

/* org_heading forward declaration */
struct org_heading;
typedef struct org_heading org_heading;

typedef struct doc_elt_ops doc_elt_ops;
extern doc_elt_ops org_heading_ops;

/* Constructor, Destructor */
org_heading *org_heading_create_empty (doc_elt_ops *elt_ops);
void org_heading_free (org_heading *self);

/* Getters and Setters */
int org_heading_get_level (org_heading *self, doc_src src);
void org_heading_set_level (org_heading *self, int level, doc_src src);

char * org_heading_get_text (org_heading *self, doc_src src);
void org_heading_set_text (org_heading *self, char *string, int length, doc_src src);

void org_heading_add_property_empty (char *string, doc_src src);

/* Adding sub elements */
void org_heading_add_subtext_last (org_heading *heading, doc_src src, org_text *text);
void org_heading_add_subheading_last (org_heading *heading, doc_src src, org_heading *text);

#endif
