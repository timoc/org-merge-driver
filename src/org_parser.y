/* Org Mode file parser -*-c-*- */
%{
#include <stdio.h>
%}

%defines

%token HEADING TEXT NUMBERED_LIST LIST NEW_LINE NEW_ELEMENT

%%
org_document: /* empty */
| org_document heading
| org_document text
| org_document numbered_list
| org_document list
| org_document NEW_ELEMENT
;

heading: HEADING TEXT NEW_LINE {printf ("HEADING ");}
| HEADING NEW_LINE {printf ("heading blank\n");}
;

numbered_list: NUMBERED_LIST TEXT NEW_LINE {printf ("NUMBERED_LIST ");}
| NUMBERED_LIST NEW_LINE
| numbered_list TEXT {printf ("nl_text ");}
| numbered_list NUMBERED_LIST {printf (" element ");}
| numbered_list NEW_LINE {printf ("nl_newline\n");}
;

list: LIST {printf ("new list\n");}
| list TEXT {printf ("list text\n");}
| list NEW_LINE

text: TEXT {printf ("new text\n");}
| text NEW_LINE

%%
