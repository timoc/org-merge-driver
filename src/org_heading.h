/**
 * @file org_heading.h
 */

#ifndef ORG_HEADING_H
#define ORG_HEADING_H

#include <stdbool.h>
#include <stdio.h>


/* org_heading forward declaration */
struct org_heading;


/* Constructor, Destructor */
struct org_heading *create_org_heading ();
void delete_org_heading (struct org_heading *self);


/* Utility Functions */
bool compare_org_heading (struct org_heading *self,
			  struct org_heading *other_heading);
void print_org_heading (struct org_heading *self, FILE *file);
void print_merge_org_heading (struct org_heading *self,
			      struct org_heading *other_heading, FILE *file);


/* Getters and Setters */
/* org_heading_level */
int get_org_heading_level (struct org_heading *self);
void set_org_heading_level (struct org_heading *self, int level);

/* org_heading_text */
char *get_org_heading_text (struct org_heading *self);
void set_org_heading_text (struct org_heading *self, char *heading_text);

#endif
