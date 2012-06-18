bin_PROGRAMS = org-merge-driver

# Bison and Flex
BUILT_SOURCES = org_parser.h org_lexer.h
AM_YFLAGS = -d
AM_LFLAGS = 

org_merge_driver_SOURCES = org_parser.y org_lexer.l main.c object.c

org_lexer.h: org_lexer.l
	lex $^
