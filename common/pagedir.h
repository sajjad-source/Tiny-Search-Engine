#ifndef __PAGEDIR_H
#define __PAGEDIR_H

#include <stdio.h>
#include "../libcs50/webpage.h"

/*
 * Initialize a page directory.
 * Creates a .crawler file in the specified directory.
 * Returns true if successful, otherwise false.
 *
 */
bool pagedir_init(const char* pageDirectory);

/*
 * Save a fetched page to the pageDirectory.
 * Writes the URL, depth, and HTML of the page to a unique file.
 */
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);

/*
 * This function examines a directory to determine if it looks like a crawler 
 * output directory, primarily by checking for the presence of a ".crawler" file.
 * 
 * Takes pageDirectory: Path to the directory that needs to be validated.
 * 
 * Returns true if the directory seems to be a valid crawler output, 
 * otherwise returns false.
 */
bool pagedir_validate(const char* pageDirectory);

/*
 * Reads a file and extracts webpage data 
 * from it, returning the constructed webpage_t object. This is typically used 
 * to load webpages that were saved by the crawler into files.
 * 
 * Takes a file pointer to the file that contains the serialized webpage.
 * 
 * Returns a pointer to the loaded webpage_t structure, or NULL if there 
 *          was an error during loading.
 */
webpage_t* pagedir_load(FILE* fp);

#endif //__PAGEDIR_H