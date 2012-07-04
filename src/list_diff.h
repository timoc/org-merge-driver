#ifdef LIST_DIFF_H
#define LIST_DIFF_H

void
list_diff (gl_list_t *list0, gl_list_t *list1, 
	   int (*compare) (void *elt1, void *elt2)
	   void (*note_insert)(gl_list_t list, size_t pos),
	   void (*note_delete)(gl_list_t list, size_t pos));

#endif
