/**
 * @file string_merge.h
 */
#ifndef STRING_MERGE_H
#define STRING_MERGE_H

/* count the number of lines in a string */
size_t count_lines (char *string, size_t length);

/* fill an array of offsets with the start of all lines in a sting.
 * The first element will be 0 and the last element will be the length of
 * the array.  Thi array must be (numer of lines + 1)
 */
void index_lines (size_t array[], char* string, size_t length);

/* merge two substrins */
void substr_print_merge (substr loc_text, substr rem_text,
			 print_ctxt *ctxt, doc_stream *out);

/* merge two strings, printing the conflicted result to a doc_stream */
void line_diff (char *loc_s, size_t loc_len, char *rem_s, size_t rem_len,
		       print_ctxt *ctxt, doc_stream *out);

typedef enum mapped_state
  {
    mapped   = 0,
    unmapped = 1
  } mapped_state;

/* calculate the if elements are mapped or not, and store the result
 * in state.  states must be initialized to 0 (false)
 */
void
string_index_compareseq (char *loc_string, size_t loc_count,
			 size_t *loc_indices, bool *loc_state,
			 char *rem_string, size_t rem_count,
			 size_t *rem_indices, bool *rem_state);

#endif /* STRING_MERGE_H */
