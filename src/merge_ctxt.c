#include "stdlib.h"
#include "string.h"
#include "config.h"
#include "gl_list.h"
#include "gl_array_list.h"
#include "smerger.h"
#include "merge_ctxt.h"

size_t default_priorities_count = 3;
const char *default_priorities[] = { "A", "B", "C" };

/**
 * @brief Create the default set of accepted priorities.
 */
static gl_list_t merge_ctxt_create_default_priorities ();

void
merge_ctxt_init (merge_ctxt *ctxt)
{
  ctxt->global_smerger = NULL;
  ctxt->priorities = NULL;
  return;
}

void
merge_ctxt_free (merge_ctxt *ctxt)
{
  smerger_free (ctxt->global_smerger);
  gl_list_free (ctxt->priorities);
  return;
}

void
merge_ctxt_set_defaults (merge_ctxt *ctxt)
{
  if (ctxt->global_smerger == NULL)
    {
      ctxt->global_smerger = smerger_create ();
    }
  if (ctxt->priorities == NULL)
    {
      ctxt->priorities = merge_ctxt_create_default_priorities ();
    }
  return;
}

static gl_list_t
merge_ctxt_create_default_priorities ()
{
  gl_list_t list =
    gl_list_nx_create_empty (GL_ARRAY_LIST, NULL, NULL, NULL, true);

  int i = 0;
  for (i = 0; i < default_priorities_count; i++)
    {
      gl_list_nx_add_last (list, default_priorities[i]);
    }

  return list;
}
