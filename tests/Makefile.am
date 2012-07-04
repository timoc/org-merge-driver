## Process this file with automake to produce Makefile.in
TESTS = check_org-merge-driver
check_PROGRAMS = check_org-merge-driver

## Check Parser
check_org_merge_driver_SOURCES = \
	check_org-merge-driver.c \
	check_parser.c \
	check_list_diff.c

check_org_merge_driver_CFLAGS = @CHECK_CFLAGS@ -I$(top_srcdir)/src \
	-I$(top_srcdir)/lib 
check_org_merge_driver_LDADD = @CHECK_LIBS@ $(top_srcdir)/lib/libgnu.a
