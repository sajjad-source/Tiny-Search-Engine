/*
 * pagedir.c    Sajjad C Kareem    October 10, 2023
 *
 * This file contains the implementations for page directory handling. Functions include:
 *     - pagedir_init: Initializes a page directory.
 *     - pagedir_save: Saves a webpage into the page directory.
 */


#include <stdio.h>
#include "pagedir.h"
#include <string.h>
#include <stdlib.h>
#include "../libcs50/file.h"

/*
 * Initialize a page directory.
 * Creates a .crawler file in the specified directory.
 * Returns true if successful, otherwise false.
 */
bool pagedir_init(const char* pageDirectory)
{
    char* filename = malloc(strlen(pageDirectory) + strlen("/.crawler") + 1);   // Allocate space for filename with added "/.crawler"
    if (filename == NULL)
    {
        return false;
    }
    sprintf(filename, "%s/.crawler", pageDirectory);                            // Add the /.crawler to the end of the pageDirectory

    /* Open the file for writing; on error, return false. */
    FILE* fp = fopen(filename, "w");

    free(filename);
    if (fp == NULL)
    {
        return false;
    }

    /* Close the file and return true. */
    fclose(fp);
    return true;
}
/*
 * Save a fetched page to the pageDirectory.
 * Writes the URL, depth, and HTML of the page to a unique file.
 */
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID)
{
    /* Allocate space for filename */
    char* filename = malloc(strlen(pageDirectory) + 20);
    if (filename == NULL)
    {
        return;
    }

    sprintf(filename, "%s/%d", pageDirectory, docID);                        // Create a page with the docID in the directory

    FILE* fp = fopen(filename, "w");
    if (fp == NULL)
    {
        free(filename);
        return;
    }
    
    /* Writes the URL, depth, and HTML of the page to a unique file. */
    fprintf(fp, "%s\n", webpage_getURL(page));
    fprintf(fp, "%d\n", webpage_getDepth(page));
    fprintf(fp, "%s\n", webpage_getHTML(page));

    fclose(fp);
    free(filename);
}

/*
 * See pagedir.h for more detail
 *
 * Takes pageDirectory: Path to the directory that needs to be validated.
 * 
 * Returns true if the directory seems to be a valid crawler output, 
 * otherwise returns false.
 */
bool pagedir_validate(const char* pageDirectory)
{
    char* filename = malloc(strlen(pageDirectory) + strlen("/.crawler") + 1);   // Allocate space for filename with added "/.crawler"

    if (filename == NULL)
    {
        return false;
    }
    sprintf(filename, "%s/.crawler", pageDirectory);                            // Add the /.crawler to the end of the pageDirectory

    FILE* fp = fopen(filename, "r");
    free(filename);

    if (fp == NULL)
    {
        return false;
    }

    fclose(fp);
    return true;
}

/*
 * See pagedir.h for more detail
 *
 * Takes a file pointer to the file that contains the serialized webpage.
 * 
 * Returns a pointer to the loaded webpage_t structure, or NULL if there 
 *          was an error during loading.
 */
webpage_t* pagedir_load(FILE* fp)
{

    if (fp == NULL)
    {
        return NULL;
    }

    char* url = file_readLine(fp);  // Retrieve the URL from the file

    int depth;
    fscanf(fp, "%d", &depth);       // Retrieve the depth from the file
    fgetc(fp);

    char* html = file_readFile(fp); // Retrieve the html content from the file

    /* Create a return a new webpage */
    webpage_t* webpage = webpage_new(url, depth, html);

    return webpage;
}