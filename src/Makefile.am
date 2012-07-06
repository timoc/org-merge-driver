bin_PROGRAMS = org-merge-driver

# Bison and Flex
BUILT_SOURCES = org_lexer.h
AM_LFLAGS = 

org_merge_driver_SOURCES = org_lexer.l main.c org_heading.c doc_merge.c
org_merge_driver_CFLAGS = -I$(top_srcdir)/gnulib/lib
org_lexer.h: org_lexer.l
	flex $^
