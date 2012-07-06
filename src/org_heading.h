/**
 * @file org_heading.h
 */

#ifndef ORG_HEADING_H
#define ORG_HEADING_H

#include "doc_elt.h"

/* org_heading forward declaration */
struct org_heading;
typedef struct org_heading org_heading;

/* Constructor, Destructor */
org_heading *org_heading_create_empty ();
void org_heading_free (org_heading *self);

/* Utility Functions */
void org_heading_print (org_heading *self, doc_stream *out);

void org_heading_print_merge (org_heading *self,
			      doc_tree_delta *delta, doc_stream *out);

bool org_heading_compare (org_heading *heading_a,
			  org_heading *heading_b);

/* Getters and Setters */
int   org_heading_get_level (org_heading *self);
void  org_heading_set_level (org_heading *self, int level);
char *org_heading_get_text (org_heading *self);
void  org_heading_set_text (org_heading *self, char *heading_text);

#endif
