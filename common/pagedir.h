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

#endif //__PAGEDIR_H