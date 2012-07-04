/**
 * @file org_element.h
 */
#ifndef ORG_ELEMENT
#define ORG_ELEMENT

#include <stdbool.h>

struct org_element;

struct org_element_operations
{
  void (* print_merge) (struct org_element *local, struct org_element *remote, 
			FILE file);
  void (* print) (struct org_element *self, FILE output_file);
  bool (* compare) (struct org_element *self, struct org_element *other_element);
};

struct org_element
{
  struct org_element_operations *operations;
};

/**
 * @brief compare two org_elements.
 */
inline bool
compare_org_element (struct org_element *self, struct org_element *other_element)
{
  assert (self->operations != NULL);
  assert (self->operations->compare != NULL);
  assert (other->operations != NULL);
  
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
      status =  self->operations->compare(self, other_elt);
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
 */
inline void
print_org_element (struct org_element *self, FILE file)
{
  if (self->operations != NULL && self->operations->print != NULL)
    {
      self->operations->print(self, file);
    }
}

/**
 * @brief print a merged org_element with conflict markers.
 * @param local
 */
inline void
print_merge_org_element (struct org_element *local,
			 struct org_element *remote, FILE file)
{

  assert (file != NULL);

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
#endif
