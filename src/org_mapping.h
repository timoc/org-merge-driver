/**
 * @file org_mapping.h
 */

#ifndef ORG_MAPPING_H
#define ORG_MAPPING_H

#include <stdlib.h>
#include <stddef.h>

struct org_element;

/* org_mapping decl */
struct org_mapping
{
  struct org_element *ancestor;
  struct org_element *local;
  struct org_element *remote;
};

enum org_mapping_offset
  {
    org_mapping_ancestor = offsetof(struct org_mapping, ancestor),
    org_mapping_local    = offsetof(struct org_mapping, local),
    org_mapping_remote   = offsetof(struct org_mapping, remote)
  };

inline struct org_element *
get_org_mapping_element (struct org_mapping * mapping, 
			 enum org_mapping_offset org_mapping_offset)
{
  char *sc = (char *)mapping;
  return ((struct org_element *)(sc + org_mapping_offset));
}

/* Constructor and Destructor */
inline struct org_mapping *
create_org_mapping ()
{
  return malloc (sizeof (struct org_mapping));
}

inline void
destroy_org_mapping (struct org_mapping *mapping)
{
  free (mapping);
}

/* Gets and Sets */

inline struct org_element *
get_org_mapping_ancestor (struct org_mapping *org_mapping)
{
  return org_mapping->ancestor;
}

inline void
set_org_mapping_ancestor (struct org_mapping *mapping, 
			  struct org_element *ancestor)
{
  mapping->ancestor = ancestor;
  return;
}


inline struct org_element *
get_org_mapping_local (struct org_mapping *mapping)
{
  return mapping->local;
}

inline void
set_org_mapping_local (struct org_mapping *mapping, struct org_element *local)
{
  mapping->local = local;
  return;
}


inline struct org_element *
get_org_mapping_remote (struct org_mapping *mapping)
{
  return mapping->remote;
}

inline void
set_org_mapping_remote (struct org_mapping *mapping, struct org_element *remote)
{
  mapping->remote = remote;
  return;
}

#endif
