
#include <stdlib.h>
#include <stdio.h>

#include "config.h"
#include <limits.h>
#include <stdbool.h>
#include "minmax.h"

#include "gl_list.h"

#define OFFSET int
#undef USE_HEURISTIC
#undef ELEMENT
#undef EQUAL

#define EXTRA_CONTEXT_FIELDS gl_list_t listx;		\
  gl_list_t listy;					\
  int (*compare) (void *elt1, void *elt2);		\
  void (*note_insert)(gl_list_t list, size_t pos);	\
  void (*note_delete)(gl_list_t list, size_t pos);

#define NOTE_DELETE(ctxt, xoff) wrap_delete (ctxt, xoff)
#define NOTE_INSERT(ctxt, yoff) wrap_insert (ctxt, yoff)
#define XVECREF_YVECREF_EQUAL(ctxt, xoff, yoff) wrap_compare(ctxt, xoff, yoff)

struct context;
static  void wrap_delete (struct context *ct, OFFSET num);
static  void wrap_insert (struct context *ct, OFFSET num);
static  int wrap_compare(struct context *ct, OFFSET xoff, OFFSET yoff);

#include "diffseq.h"

#define OFFSET int

static  void
wrap_delete (struct context *ct, OFFSET num)
{
  ct->note_delete (ct->listx, num);
  return;
}

static  void
wrap_insert (struct context *ct, OFFSET num)
{
  ct->note_insert (ct->listy, num);
  return;
}

static  int
wrap_compare (struct context *ct, OFFSET xoff, OFFSET yoff)
{
  const void *elt1, *elt2;
  elt1 = gl_list_get_at (ct->listx, xoff);
  elt2 = gl_list_get_at (ct->listy, yoff);
  return ct->compare ((void*)elt1, (void*)elt2);
}

/* Create the diffseq for the elements of a list */
void
list_diff (gl_list_t list0, gl_list_t list1, 
	   int (*compare) (void *elt1, void *elt2),
	   void (*note_insert)(gl_list_t list, size_t pos),
	   void (*note_delete)(gl_list_t list, size_t pos))
{
  struct context ctxt;

  /* Add the caller data */
  ctxt.listy = list1;
  ctxt.listx = list0;
  ctxt.note_delete = note_delete;
  ctxt.note_insert = note_insert;
  ctxt.compare = compare;

  /* Allocate data for the algorithm */
  size_t diags = (gl_list_size (list0)
		  + gl_list_size (list1) + 3);
  void *mem = malloc (diags * (2 * sizeof (OFFSET)));
  ctxt.fdiag = mem;
  ctxt.bdiag = ctxt.fdiag + diags;

  ctxt.fdiag += gl_list_size(list1) + 1;
  ctxt.bdiag = ctxt.fdiag + diags;

  /* Set TOO_EXPENSIVE to be approximate square root of input size,
     bounded below by 256.  */
  /*
    too_expensive = 1;
    for (;  diags != 0;  diags >>= 2)
    too_expensive <<= 1;
    ctxt.too_expensive = MAX (256, too_expensive);
  */

  compareseq (0, // array starts at 0
	      gl_list_size (list0),
	      0,
	      gl_list_size (list1),
	      1,
	      &ctxt);

  free (mem);
  return;
}
