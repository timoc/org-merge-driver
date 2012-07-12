/**
 * @file debug.h
 * #define DEBUG_FILE "debug.h"
 */

#ifndef DEBUG_H
#define DEBUG_H

/* Enable debug globally */
#ifndef DEBUG
#define DEBUG
#endif /* DEBUG */

/* DEBUG_ON is used to test if debug is enabled */
#ifdef DEBUG
#define DEBUG_ON 1
#else
#define DEBUG_ON 0
#endif /* USE_DEBUG */

/* Utility Macros */
#define XCONCAT(a,b) a ## b
#define CONCAT(a,b) XCONCAT(a,b)
#define XQUOTE(str) #str
#define QUOTE(str) XQUOTE(str)
#define EVAL(val) val

#ifndef DEBUG_OUT 
#define DEBUG_OUT stderr
#endif /* DEBUG_OUT */

#ifndef DEBUG_PRINT
#define DEBUG_PRINT 1
#endif /* DEBUG_PRINT */

/* Standard Debug Module  */
#ifndef DEBUG_MODULE
#define DEBUG_MODULE DEFAULT
#endif /* DEBUG_MODULE */

#define DEFAULT_DEBUG_ON 0
#define MAIN_DEBUG_ON 1

#define debug_printf(format, ...)				\
  do { if (DEBUG_ON && CONCAT(DEBUG_MODULE,_DEBUG_ON)) {	\
      fprintf (DEBUG_OUT,					\
	       QUOTE(DEBUG_MODULE) ":%s:%s:%d: " format,	\
	       __FILE__, __func__, __LINE__,			\
	       ##__VA_ARGS__); } } while (0)
  

#endif /*DEBUG_H */
