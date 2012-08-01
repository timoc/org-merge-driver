/**
 * @file smerger.h 
 *
 * This file holds the public interface for smerger, the search
 * merger object. An smerger is used to match and merge document
 * elements that cannot be matched locally by their parents.  In
 * general, Elements that can be moved must be matched and merged
 * using a document-wide search merger.
 */

#ifndef SMERGER_H
#define SMERGER_H

/**
 * A search merger object. This object stores a database of document
 * elements which it tries to match and merge.
 */
typedef struct smerger smerger;

/**
 * Create a new empty instance of a smerger.
 */
smerger *smerger_create ();

/**
 * Free an existing instance of an smerger object.
 */
void smerger_free (smerger *sm);

/**
 * Register a doc_elt with an smerger object. If a match is found
 * for a doc_elt, the smerger will remove both elements from the
 * database, and attempt to merge them through the doc_elt generic
 * interface.
 */
int smerger_register_insert (smerger *sm, doc_ref *ref, merge_ctxt *ctxt);
int smerger_register_delete (smerger *sm, doc_ref *ref, merge_ctxt *ctxt);

/**
 * Unregister a doc_elt from an smerger object.
 */
int smerger_unregister_insert (smerger *sm, doc_ref *ref);
int smerger_unregister_delete (smerger *sm, doc_ref *ref);

#endif
