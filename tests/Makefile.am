## Process this file with automake to produce Makefile.in
TESTS = check_org-merge-driver check_merge_rules.sh

## Check Unit Tests
check_PROGRAMS = check_org-merge-driver

check_org_merge_driver_SOURCES = \
	check_org-merge-driver.c

check_org_merge_driver_CFLAGS = @CHECK_CFLAGS@ -I$(top_srcdir)/src \
	-I$(top_srcdir)/gnulib/lib
check_org_merge_driver_LDADD = @CHECK_LIBS@ $(top_srcdir)/gnulib/lib/libgnu.a

## Merge Rule Tests
#check_org-merge-driver
TEST_EXTENSIONS = .sh
SH_LOG_DRIVER = env AM_TAP_AWK='$(AWK)' $(SHELL) \
                       $(top_srcdir)/config/tap-driver.sh

# retangle the test files if the tests are updated
check_merge_rules.sh : test.org
	echo "Must re-tangle files"
	emacs --batch --file test.org --eval "(org-babel-tangle)"
	chmod 700 check_merge_rules.sh # make the generated test script executable
