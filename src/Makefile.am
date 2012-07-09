bin_PROGRAMS = org-merge-driver

# Bison and Flex
BUILT_SOURCES = org_lexer.h
AM_LFLAGS = 

org_merge_driver_SOURCES =  main.c \
	doc_merge.c \
	org_lexer.l \
	org_parser.c \
	list_diff.c \
	org_heading.c \
	org_text.c \
	merge_print.c

org_merge_driver_CFLAGS = -O1 -I$(top_srcdir)/gnulib/lib
org_merge_driver_LDADD = $(top_srcdir)/gnulib/lib/libgnu.a

org_lexer.h: org_lexer.l
	flex $^
