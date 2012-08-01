/**
 * @file doc_ref.h
 */

#ifndef DOC_REF_H
#define DOC_REF_H

#include <stdlib.h>
#include "gl_list.h"
#include "doc_stream.h"
//#include "doc_elt.h"

/* #include "doc_elt.h" */
struct doc_elt;
typedef struct doc_elt doc_elt;
typedef struct merge_ctxt merge_ctxt;

/**
 * Indicates an input document (ancestor, local, or source). Used to
 * indicate the document from which a doc_* object originates.
 */
typedef enum doc_src
  {
    ANC_SRC = 1,
    REM_SRC = 2,
    LOC_SRC = 4
  } doc_src;

/**
 * A reference to a doc_elt.  It is used to keep track of what files a
 * merged document represents.
 */
typedef struct doc_ref
{
  doc_src src;
  doc_elt *elt;
} doc_ref;

/* Constructor and destructor */
static inline doc_ref *doc_ref_create_empty ();
static inline void doc_ref_free (doc_ref *ref);

/* Set the doc_elt reference */
static inline doc_elt * doc_ref_get_elt (doc_ref *ref);
static inline void doc_ref_set_elt (doc_ref *ref, doc_elt *elt);

/* Set the doc_src this file represents */
static inline doc_src doc_ref_get_src (doc_ref *ref);
static inline void doc_ref_set_src (doc_ref *ref, doc_src src);
static inline void doc_ref_add_src (doc_ref *ref, doc_src src);
static inline bool doc_ref_isexactly (doc_ref *ref, doc_src src);
static inline bool doc_ref_contains (doc_ref *ref, doc_src src);

/*
 * doc_reflist
 */
/**
 * @brief Check to see if any of the elemets in a list of doc_refs are
 * updated.
 * This depends on the element's implementation of this.
 * This function will stop early if it finds a response.
 */
bool doc_reflist_isupdated (gl_list_t reflist);

/**
 * @brief Merge two doc_ref lists together.  This function changes ancestor.
 */
void doc_reflist_merge (gl_list_t ancestor, gl_list_t descendant, merge_ctxt *ctxt);

/**
 * @brief print a list of ref_docs
 */
void doc_reflist_print (gl_list_t reflist, void *context, doc_stream *out);

/*
 * Implementation Details
 */

static inline doc_ref *
doc_ref_create_empty ()
{
  doc_ref *d = malloc (sizeof (doc_ref));
  d->elt = NULL;
  d->src = 0;
  return d;
}

static inline void
doc_ref_free (doc_ref *ref)
{
  free (ref);
}

static inline doc_elt *
doc_ref_get_elt (doc_ref *ref)
{
  return ref->elt;
}

static inline void
doc_ref_set_elt (doc_ref *ref, doc_elt *elt)
{
  ref->elt = elt;
  return;
}

static inline doc_src
doc_ref_get_src (doc_ref *ref)
{
  return ref->src;
}

static inline void
doc_ref_set_src (doc_ref *ref, doc_src src)
{
  ref->src = src;
  return;
}

static inline void
doc_ref_add_src (doc_ref *ref, doc_src src)
{
  ref->src = ref->src | src;
  return;
}

static inline bool
doc_ref_isexactly (doc_ref *ref, doc_src src)
{
  return (ref->src == src);
}

static inline bool
doc_ref_contains (doc_ref *ref, doc_src src)
{
  return (ref->src & src);
}

#endif /* DOC_REF_H */
