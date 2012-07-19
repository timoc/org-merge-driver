/**
 * @file org_heading.h
 */

#ifndef ORG_HEADING_H
#define ORG_HEADING_H

/* org_text forward declaration */
struct org_text;
typedef struct org_text org_text;

/* org_heading forward declaration */
struct org_heading;
typedef struct org_heading org_heading;

/* Constructor, Destructor */
org_heading *org_heading_create_empty ();
void org_heading_free (org_heading *self);

/* Getters and Setters */
int   org_heading_get_level (org_heading *self);
void  org_heading_set_level (org_heading *self, int level);
char *org_heading_get_text (org_heading *self);
void  org_heading_set_text (org_heading *self, char *heading_text);
void org_heading_set_have_property_list (org_heading *h, bool have_one);
void org_heading_add_property_empty (char *string);
org_text *org_heading_get_text_elt (org_heading *self);
void org_heading_set_text_elt (org_heading *self, org_text * text);

#endif
