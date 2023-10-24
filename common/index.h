#ifndef __INDEX_H
#define __INDEX_H

#include <stdbool.h>
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"

typedef struct index index_t;

/* Create a new index data structure with a given number of slots for the hashtable */
index_t* index_new(int num_slots);

/* Insert a word and its associated counters into the index */
bool index_insert(index_t* index, char* word, counters_t* ctrs);

/* Delete the index and free all associated memory */
void index_delete(index_t* index);

/* Save the index to the specified file. */
void index_save(index_t* index, const char* filename);

/* 
 * Load the index from the specified file.
 * Returns pointer to the loaded index, or NULL if any error.
 */
index_t* index_load(FILE* fp);

/* 
 * Find the counters associated with the specified word in the index.
 * Returns pointer to the counters if found, otherwise NULL.
 */
counters_t* index_find(index_t* index, char* word);

/* 
 * Delete the index and free all associated memory.
 */
void item_delete(void* item);

/* 
 * Print the counters in the specified format.
 * Used as a helper function that is passed to index_print
 * when calling counters_iterate
 */
void ctr_print(void* arg, int key, int count);

/* 
 * Print an index item
 * Used as a helper function that is passed to index_save
 * when calling hashtable_iterate to print to a specified file
 */
void index_print(void* arg, const char* key, void* item);


#endif // __INDEX_H