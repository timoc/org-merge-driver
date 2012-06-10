#include <stdlib.h>
#include <check.h>

START_TEST (parser_check)
{
  fail_if (1 > 2);
}
END_TEST

Suite *
parser_suite (void)
{
  Suite *s = suite_create ("Parsing");
  /* Core test case */
  TCase *tc_core = tcase_create ("Core");
  tcase_add_test (tc_core, parser_check);
  suite_add_tcase (s, tc_core);
  return s;
}

int
main (void)
{
  int number_failed;
  Suite *s = parser_suite ();
  SRunner *sr = srunner_create (s);
  srunner_run_all (sr, CK_NORMAL);
  number_failed = srunner_ntests_failed (sr);
  srunner_free (sr);

  return (number_failed == 0) 
    ? EXIT_SUCCESS 
    : EXIT_FAILURE;
}
