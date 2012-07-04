/**
 * @file org_mapping.h
 */

#ifndef ORG_MAPPING_H
#define ORG_MAPPING_H


struct org_element;

/* org_mapping decl */
struct org_mapping
{
  struct org_element *ancestor;
  struct org_element *local;
  struct org_element *remote;
};


/* Gets and Sets */

inline struct org_element *
get_org_mapping_ancestor (struct mapping *org_mapping)
{
  return mapping->ancestor;
}

inline void
set_org_mapping_ancestor (struct org_mapping *mapping, struct org_element *ancestor)
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
set_org_mapping_remote (struct org_mapping *mapping, sturct org_element *remote)
{
  mapping->remote = remote;
  return;
}

#endif
