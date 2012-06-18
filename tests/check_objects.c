#include <stdlib.h>
#include <check.h>
#include "object.c"

static int state=0;

static void *
Empty_ctor (void * _self, va_list * app)
{
  state+=1;
  return _self;
}

static void *
Empty_dtor (void * _self)
{
  state+=1;
  return _self;
}

START_TEST (new_object_call_constructor)
{
  struct Empty
  {
    const struct Object _;
  };
  struct EmptyClass
  {
    const struct Class _;
  };
  void * EmptyClass = new(Class, "EmptyClass",
			  Class, sizeof(struct EmptyClass),
			  0);
  void * Empty = new(EmptyClass, "Empty",
		     Object, sizeof(struct Empty),
		     ctor, Empty_ctor,
		     0);
  state = 0;
  void *e = new(Empty);
  fail_if(state != 1);
  delete(e);
}
END_TEST

START_TEST (delete_object_call_destructor)
{
  struct Empty
  {
    const struct Object _;
  };
  struct EmptyClass
  {
    const struct Class _;
  };
  void * EmptyClass = new(Class, "EmptyClass",
			  Class, sizeof(struct EmptyClass),
			  0);
  void * Empty = new(EmptyClass, "Empty",
		     Object, sizeof(struct Empty),
		     ctor, Empty_dtor,
		     0);
  state = 0;
  void *e = new(Empty);
  delete(e);
  fail_if(state != 1);
}
END_TEST

START_TEST(delete_class_stop_from_destroying)
{
  struct Empty
  {
    const struct Object _;
  };
  struct EmptyClass
  {
    const struct Class _;
  };
  void * EmptyClass = new(Class, "EmptyClass",
			  Class, sizeof(struct EmptyClass),
			  0);
  delete (EmptyClass);
  /* should fail if the test was deleted */
  void * Empty = new(EmptyClass, "Empty",
		     Object, sizeof(struct Empty),
		     dtor, Empty_dtor,
		     0);
  fail_if (Empty==0);
}
END_TEST

START_TEST(differ_different_classes_differ)
{
  void * AClass = new(Class, "AClass",
		      Class, sizeof(struct Class),
		      0);
  void * BClass = new(Class, "BClass",
		      Class, sizeof(struct Class),
		      0);
  void * A = new(AClass, "A",
		 Object, sizeof(struct Object),
		 0);
  void * B = new(BClass, "B",
		 Object, sizeof(struct Object),
		 0);

  fail_unless (differ (A, B));
}
END_TEST

START_TEST(differ_same_classes_do_not_differ)
{
  void * A = new(Class, "A",
		 Class, sizeof(struct Class),
		 0);
  fail_if (differ (A, A));
}
END_TEST

Suite *
make_object_suite (void)
{
  Suite *s = suite_create ("Object");
  /* Core test case */
  TCase *tc_core = tcase_create ("Core");
  tcase_add_test (tc_core, new_object_call_constructor);
  tcase_add_test (tc_core, delete_object_call_destructor);
  tcase_add_test (tc_core, delete_class_stop_from_destroying);
  tcase_add_test (tc_core, differ_different_classes_differ);
  tcase_add_test (tc_core, differ_same_classes_do_not_differ);
  suite_add_tcase (s, tc_core);
  return s;
}
