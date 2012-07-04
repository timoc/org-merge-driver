
#include <stdlib.h>
#include <stdio.h>

#include "config.h"
#include <limits.h>
#include <stdbool.h>
#include "minmax.h"

#include "gl_list.h"

#define OFFSET size_t
#undef USE_HEURISTIC
#define EXTRA_CONTEXT_FIELDS gl_list_t listx;		\
  gl_list_t listy;					\
  int (*compare) (void *elt1, void *elt2);		\
  void (*note_insert)(gl_list_t list, size_t pos);	\
  void (*note_delete)(gl_list_t list, size_t pos);

#define NOTE_DELETE(ctxt, xoff) wrap_delete (ctxt, xoff)
#define NOTE_INSERT(ctxt, yoff) wrap_insert (ctxt, yoff)
#define XVECREF_YVECREF_EQUAL(ctxt, xoff, yoff) wrap_compare(ctxt, xoff, yoff)

struct context;
static inline void wrap_delete (struct context *ct, OFFSET num);
static inline void wrap_insert (struct context *ct, OFFSET num);
static inline int wrap_compare(struct context *ct, OFFSET xoff, OFFSET yoff);

#include "diffseq.h"
#define OFFSET size_t

/* Create the diffseq for the elements of a list */
void
list_diff (gl_list_t list0, gl_list_t list1, 
	   int (*compare) (void *elt1, void *elt2),
	   void (*note_insert)(gl_list_t list, size_t pos),
	   void (*note_delete)(gl_list_t list, size_t pos))
{
  struct context ct;
  ct.listy = list1;
  ct.listx = list0;
  ct.note_delete = note_delete;
  ct.note_insert = note_insert;
  ct.compare = compare;

  compareseq (0, // array starts at 0
	      gl_list_size (list0),
	      0,
	      gl_list_size (list1),
	      0,
	      &ct);
  return;
}

static inline void
wrap_delete (struct context *ct, OFFSET num)
{
  ct->note_delete (ct->listx, num);
  return;
}

static inline void
wrap_insert (struct context *ct, OFFSET num)
{
  ct->note_insert (ct->listy, num);
  return;
}

static inline int
wrap_compare (struct context *ct, OFFSET xoff, OFFSET yoff)
{
  const void *elt1, *elt2;
  elt1 = gl_list_get_at (ct->listx, xoff);
  elt2 = gl_list_get_at (ct->listy, yoff);
  return ct->compare ((void*)elt1, (void*)elt2);
}
