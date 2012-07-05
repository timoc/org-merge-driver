/**
 * @file org_element.h
 */
#ifndef ORG_ELEMENT_H
#define ORG_ELEMENT_H

#include <stdbool.h>
#include <assert.h>
#include <stdio.h>

struct org_element;

struct org_element_operations
{
  void (* print_merge) (struct org_element *local, struct org_element *remote, 
			FILE *file);
  void (* print) (struct org_element *self, FILE *file);
  bool (* compare) (struct org_element *self, struct org_element *other_element);
};

struct org_element
{
  const struct org_element_operations *operations;
};

/**
 * @brief Compare two org_elements.  
 * @self Compare this element. Uses this elements operations.
 * @other_erement The element to compare with.
 *
 * two org_elements, returning TRUE if they match each other by some
 * distiguishing factor, false otherwise.  It is okay to compare to
 * elements with different operations.
 */
static inline bool
compare_org_element (struct org_element *self, struct org_element *other_element)
{
  assert (self->operations != NULL);
  assert (self->operations->compare != NULL);
  assert (other_element->operations != NULL);
  
  bool status = false;
  /* If two elements share the same set of operations, then they are
   * comparable and of the same type. The deffered compare function
   * will assume both elements are of the same type.
   */

  /* Test that both elements share the same operations structure */
  if (self->operations == other_element->operations)
    {
      /* If the elements are of the same type, compare them by calling the
	 self org_element's compare implementation on both element. */
      status =  self->operations->compare(self, other_element);
    }
  else
    {
      /* If the elements are not of the same type [ie don't share an
       * operations struct] then they cannot be equal.  Return
       * false */
      return false;
    }
}

/**
 * @brief print an org_elment to file.
 * @parem self The element to print.
 * @param file The FILE stream to print the element to.
 */
static inline void
print_org_element (struct org_element *self, FILE *file)
{
  assert (self->operations != NULL && self->operations->print != NULL);
  self->operations->print(self, file);
}

/**
 * @brief print a merged org_element with conflict markers.
 * @param local The locas version of a file.
 * @param remote The remote version of a file.
 * @param file The file stream to print to.
 */
static inline void
print_merge_org_element (struct org_element *local,
			 struct org_element *remote, FILE *file)
{

  assert (file != 0);

  /* test local and remote org elements for null.
   * If either is null, do a simple print of the non-null element
   * If both are null, print nothing
   */
  if (local !=NULL)
    {
      if (remote != NULL)
	{
	  /* neither local or remote is null, merge print */
	  assert (local->operations != NULL);
	  assert (local->operations->print_merge != NULL);
	  assert (local->operations == remote->operations);

	  local->operations->print_merge (local, remote, file);
	}
      else
	{
	  /* remote is null, print local org element without merge*/
	  print_org_element (local, file);
	}
    }
  else if (remote != NULL)
    {
      /* local is null, print remote org element without merge*/
      print_org_element (remote, file);
    }
}

/**
 * @brief set an org_element's operations table.
 * @param self The element to assign an org_element_operations to.
 * @param operatios The operations table to be assigned to self.
 */
static inline void
set_org_element_operations (struct org_element *self, 
			    const struct org_element_operations *operations)
{
  self->operations = operations;
  return;
}

#endif
