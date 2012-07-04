/**
 * @file org_heading.c
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "org_element.h"
#include "org_heading.h"

/* Forward Declarations */
struct org_heading_operations;
static bool compare_org_heading_op (struct org_element *self,
				    struct org_element *other_element);
static void print_org_heading_op (struct org_element *self, FILE *file);
static void print_merge_org_heading_op (struct org_element *local, 
					struct org_element *remote, 
					FILE *file);

/* Implementation of org_element operations */

static const struct org_element_operations org_heading_operations = {
  .print        = print_org_heading_op,
  .compare      = compare_org_heading_op,
  .print_merge  = print_merge_org_heading_op,
};

/**
 * @brief call compare_org_heading on two org_element.
 */
static bool
compare_org_heading_op (struct org_element *self,
			struct org_element *other_element)
{
  assert (self != NULL && other_element != NULL);
  assert (self->operations == &org_heading_operations);
  assert (other_element->operations == &org_heading_operations);

  return compare_org_heading ((struct org_heading *)self,
			      (struct org_heading *)other_element);
}

/**
 * @brief call print_org_heading on an org_element.
 */
static void
print_org_heading_op (struct org_element *self, FILE *file)
{
  assert (self != NULL);
  assert (self->operations == &org_heading_operations);
  print_org_heading ((struct org_heading *)self, file);
  return;
}

/**
 * @brief call print_org_heading on two org_element;
 */
static void
print_merge_org_heading_op (struct org_element *local, 
			    struct org_element *remote, 
			    FILE *file)
{
  assert (local != NULL);
  assert (local-> operations == &org_heading_operations);
  assert (remote->operations == &org_heading_operations);

  print_merge_org_heading ((struct org_heading *) local,
			   (struct org_heading *) remote, file);
  return;
}


/* Core org_heading implementation */

struct org_heading
{
  struct org_element org_element;
  int level;
  char *pre_heading_text;
  char *heading_text;
  char *post_heading_text;
  char *body_text;
  char * uid_string;
};


/* Constructor, Destructor */
struct org_heading *
init_org_heading ()
{
  struct org_heading *new_heading = malloc ( sizeof (struct org_heading));
  new_heading->org_element.operations = &org_heading_operations;
}

void
delete_org_heading (struct org_heading *self)
{
  free (self);
}

/* Utility functions */
bool compare_org_heading (struct org_heading *self,
			  struct org_heading *other_heading)
{
  bool status = false;
  if (strcmp (self->heading_text, other_heading->heading_text))
    {
      status = true;
    }
  return status;
}

void print_org_heading (struct org_heading *self, FILE *file)
{
  /**
   * @todo Implement print_org_heading.
   */
}

void print_merge_org_heading (struct org_heading *self,
			      struct org_heading *other_heading, FILE *file)
{
  /**
   * @todo Implement print_merge_org_heading.
   */
}


/* Getters and Setters */
/* org_heading_level */
int get_org_heading_level (struct org_heading *self)
{
  return self->level;
}

void set_org_heading_level (struct org_heading *self, int level)
{
  self->level = level;
}

/* org_heading_text */
void set_org_heading_text (struct org_heading *self, char *heading_text)
{
  self->heading_text = heading_text;
}

char *get_org_heading_text (struct org_heading *self)
{
  return self->heading_text;
}
