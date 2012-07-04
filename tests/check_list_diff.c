#include "list_diff.c"
#include <stdlib.h>
#include <check.h>
#include "gl_array_list.h"

gl_list_t list;
size_t pos;

void
test_delete (gl_list_t l, size_t p)
{
  list = l;
  pos = p;
}

int test_compare (void* elt1, void *elt2)
{
  return ((int)elt1 + 2*(int)elt2);
}

START_TEST (wrap_delete_calls_delete_with_listx)
{
  struct context ct;
  ct.note_delete = test_delete;
  ct.listx = (void*) 1;
  list = 0;
  pos = 0;
  wrap_delete (&ct, 8);
  fail_unless(list == (void*) 1);
}
END_TEST

START_TEST (wrap_delete_calls_delete_with_pos)
{
  struct context ct;
  ct.note_delete = test_delete;
  ct.listx = (void*) 1;
  list = 0;
  pos = 0;
  wrap_delete (&ct, 8);
  fail_unless(pos == 8);
}
END_TEST

START_TEST (wrap_insert_calls_delete_with_listy)
{
  struct context ct;
  ct.note_insert = test_delete;
  ct.listy = (void*) 1;
  list = 0;
  pos = 0;
  wrap_insert (&ct, 8);
  fail_unless(list == (void*) 1);
}
END_TEST

START_TEST (wrap_insert_calls_delete_with_pos)
{
  struct context ct;
  ct.note_insert = test_delete;
  ct.listx = (void*) 1;
  list = 0;
  pos = 0;
  wrap_insert (&ct, 8);
  fail_unless(pos == 8);
}
END_TEST

START_TEST (compare_calls_given_elements)
{
  struct context ct;
  gl_list_t gl = gl_list_nx_create_empty (GL_ARRAY_LIST, NULL, 
				       NULL, NULL, true);
  gl_list_nx_add_last (list, (void*) 1);
  gl_list_nx_add_last (list, (void*) 2);
  ct.compare = test_compare;
  ct.listx = gl;
  ct.listy = gl;

  fail_unless(wrap_compare (&ct, 0, 1) == 5);
}
END_TEST

START_TEST (list_diff_check)
{
  /*
gl_list_t l1 =
void
list_diff (gl_list_t list0, gl_list_t list1, 
	   int (*compare) (void *elt1, void *elt2),
	   void (*note_insert)(gl_list_t list, size_t pos),
	   void (*note_delete)(gl_list_t list, size_t pos))
  */
  fail_if (1 > 2);
}
END_TEST


Suite *
make_list_diff_suite (void)
{
  Suite *s = suite_create ("List Diff");
  /* Core test case */
  TCase *tc_core = tcase_create ("Core");
 
  tcase_add_test (tc_core, wrap_delete_calls_delete_with_listx);
  tcase_add_test (tc_core, wrap_delete_calls_delete_with_pos);
  tcase_add_test (tc_core, wrap_insert_calls_delete_with_listy);
  tcase_add_test (tc_core, wrap_insert_calls_delete_with_pos);
  tcase_add_test (tc_core, compare_calls_given_elements);
  tcase_add_test (tc_core, list_diff_check);

  suite_add_tcase (s, tc_core);
  return s;
}

