#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <check.h>
#include "doc_elt.h"

#include "org_heading.h"
#include "org_heading.c"


#include "org_phony.h"
#include "org_phony.c"

/*
 * Test Case Org Phony
 */

START_TEST (check_org_phony_compare_op)
{
  org_phony *phony_a = org_phony_create('A', true);
  org_phony *phony_b = org_phony_create('B', false);
  fail_unless ((doc_elt_compare ((doc_elt*)phony_a, (doc_elt*)phony_b)));
}
END_TEST

/*
 * Test Case Org Heading
 */
START_TEST (check_compare_org_header_are_same)
{
  char heading_text[] = "Test Heading Text";
  org_heading * h = org_heading_create_empty();
  org_heading_set_level (h, 5);
  org_heading_set_text (h, heading_text);
  fail_unless ((doc_elt_compare ((doc_elt*)h, (doc_elt*)h)));
}
END_TEST

START_TEST (check_compare_org_header_are_different_levels)
{
  char heading_text[] = "Test Heading Text";
  org_heading * h1 = org_heading_create_empty();
  org_heading * h2 = org_heading_create_empty();

  org_heading_set_level (h1, 1);
  org_heading_set_text (h1, heading_text);

  org_heading_set_level (h2, 3);
  org_heading_set_text (h2, heading_text);

  fail_if ((doc_elt_compare ((doc_elt*)h1, (doc_elt*)h2)));
}
END_TEST

START_TEST (check_compare_org_header_are_different_text)
{
  char heading_text1[] = "Test Heading 1 Text";
  char heading_text2[] = "Test Heading 2 Text";
  org_heading * h1 = org_heading_create_empty();
  org_heading * h2 = org_heading_create_empty();

  org_heading_set_level (h1, 1);
  org_heading_set_text (h1, heading_text1);

  org_heading_set_level (h2, 1);
  org_heading_set_text (h2, heading_text2);

  fail_if ((doc_elt_compare ((doc_elt*)h1, (doc_elt*)h2)));
}
END_TEST


/*
 * Org Elements Suite
 */

Suite *
make_org_elements_suite (void)
{

  Suite *s = suite_create ("Org Elements");
  /* Core test case */
  TCase *tc_core = tcase_create ("Core");
 
  tcase_add_test (tc_core, check_org_phony_compare_op);
  tcase_add_test (tc_core, check_compare_org_header_are_same);
  tcase_add_test (tc_core, check_compare_org_header_are_different_levels);
  tcase_add_test (tc_core, check_compare_org_header_are_different_text);

  suite_add_tcase (s, tc_core);
  return s;

}
