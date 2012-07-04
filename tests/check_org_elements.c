#include <stdlib.h>
#include <stdio.h>
#include <check.h>
#include "org_element.h"
#include "org_heading.h"
#include "org_heading.c"

START_TEST (check_print_org_header)
{
  char heading_text[] = "Test Heading Text";
  int heading_level = 4;
  struct org_heading *heading = create_org_heading ();
  set_org_heading_level (heading, heading_level);
  set_org_heading_text  (heading, heading_text);

  print_org_heading (heading, stdout);
  fail_if (0);
}
END_TEST


Suite *
make_org_elements_suite (void)
{
  Suite *suite = suite_create ("Org Elements");

  /* Core test case */
  TCase *tc_org_header = tcase_create ("Org Header");
  
  tcase_add_test (tc_org_header, check_print_org_header);

  suite_add_tcase (suite, tc_org_header);
  return suite;
}
