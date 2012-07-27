/**
 * @file doc_stream.h
 */

#ifndef DOC_STREAM_H
#define DOC_STREAM_H

#include <stdio.h>

/**
 * @todo write safe wrapper macros.
 */

#define doc_stream FILE
#define doc_stream_open fopen
#define doc_stream_close fclose
#define doc_stream_putc putc
#define doc_stream_puts fputs

#endif
