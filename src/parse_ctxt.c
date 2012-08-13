#include "stdlib.h"
#include "string.h"
#include "config.h"
#include "debug.h"
#include "gl_list.h"
#include "gl_array_list.h"
#include "parse_ctxt.h"


static const size_t default_todo_states_size = 2;
static const char* default_todo_states[] = {"TODO", "DONE"};

gl_list_t parse_ctxt_create_default_todo_states ();

void
parse_ctxt_init (parse_ctxt *ctxt)
{
  ctxt->todo_states = NULL;
  return;
}

void
parse_ctxt_set_defaults (parse_ctxt *ctxt)
{
  if (ctxt->todo_states == NULL)
    {
      ctxt->todo_states = parse_ctxt_create_default_todo_states ();
    }

  return;
}

gl_list_t
parse_ctxt_create_default_todo_states ()
{
  gl_list_t list =
    gl_list_nx_create_empty (GL_ARRAY_LIST, NULL, NULL, NULL, true);

  int i = 0;
  for (i = 0; i < default_todo_states_size; i++)
    {
      gl_list_nx_add_last (list, default_todo_states[i]);
    }

  return list;
}
