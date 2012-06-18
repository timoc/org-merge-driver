## Process this file with automake to produce Makefile.in
TESTS = check_org-merge-driver
check_PROGRAMS = check_org-merge-driver

## Check Parser
check_org_merge_driver_SOURCES = \
	check_org-merge-driver.c \
	check_objects.c \
	check_parser.c

check_org_merge_driver_CFLAGS = @CHECK_CFLAGS@ -I$(top_srcdir)/src 
check_org_merge_driver_LDADD = @CHECK_LIBS@
