#include "doc_merge.c"
#include <stdlib.h>
#include <check.h>

START_TEST (one)
{

  /*
  gl_list_t gl = gl_list_nx_create_empty (GL_ARRAY_LIST, NULL, 
					  NULL, NULL, true);
  gl_list_nx_add_last (gl, &one);
  gl_list_nx_add_last (gl, &two);
  */

  fail_unless (1);
}
END_TEST

Suite *
make_doc_merge_suite (void)
{
  Suite *s = suite_create ("Doc Merge");
  /* Core test case */
  TCase *tc_core = tcase_create ("Core");
 
  tcase_add_test (tc_core, one);

  suite_add_tcase (s, tc_core);
  return s;
}
