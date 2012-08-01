/**
 *
 */
#ifndef DOC_ELT_UTIL
#define DOC_ELT_UTIL

#include <stdbool.h>
#include <string.h>
#include "debug.h"

#include "doc_stream.h"
#include "doc_ref.h"

/*
 * Data index enumeration
 */

typedef enum data_index
  {
    ANC_INDEX = 2, /* must be the highest */
    LOC_INDEX = 1,
    REM_INDEX = 0
  } data_index;

static inline data_index
srctoindex (doc_src src)
{
  data_index i = ANC_INDEX;
 switch (src)
   {
   case ANC_SRC:
     i = ANC_INDEX;
     break;
   case LOC_SRC:
     i = LOC_INDEX;
     break;
   case REM_SRC:
     i = REM_INDEX;
     break;
   default:
     abort ();
     break;
   }
 return i;
}

/*
 * Substrings
 */

typedef struct substr substr;
static int  substrcmp(substr a, substr b);
static bool substreql(substr a, substr b);

typedef struct substr
{
  char *string;
  size_t length;
} substr;

static inline int
substrcmp(substr a, substr b)
{
  int length = (a.length < b.length) ? a.length : b.length;
  return strncmp(a.string, b.string, length);
}



static inline void
substrprint(substr s, doc_stream *out)
{
  /**
   * @todo Implement with a doc_stream function.
   */

  fwrite (s.string, sizeof (char), s.length, out);
  return;
}

static inline bool
substreql(substr a, substr b)
{
  bool eql = false;
  debug_msg (DOC_ELT, 5, "a.length=%d b.length=%d  ", a.length, b.length);
  if (DOC_ELT_PRINTLEVEL >= 5)
    {
      substrprint(a, stderr);
      substrprint(b, stderr);
      fputs("\n", stderr);
    }

  if (a.length == b.length)
    {
      eql = (substrcmp(a, b) == 0);
    }
  return eql;
}

/*
 * Conflict Marker Printing
 */

#endif
