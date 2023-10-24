/*
 * indexer.c    Sajjad C Kareem    October 17, 2023
 *
 * This file contains the definitions for building an index from 
 * crawled web pages. 
 * Functions include:
 *     - indexBuild: Builds the index by loading each webpage from a directory 
 *                   and processing words from each page.
 *     - indexPage: Processes each word in a webpage and updates the index.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libcs50/webpage.h"
#include "../common/word.h"
#include "pagedir.h"
#include "index.h"

/* Function declarations */
void indexBuild(const char* pageDirectory, const char* indexFilename);
void indexPage(index_t* index, webpage_t* webpage, int docID);

int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        printf("Usage: ./indexer pageDirectory indexFilename\n");
        return 1;
    }

    const char* pageDirectory = argv[1];
    const char* indexFilename = argv[2];

    // Build index from the given page directory
    indexBuild(pageDirectory, indexFilename);

    return 0;
}

/*
 * indexBuild: Builds the index from crawled web pages located 
 * in a given directory and saves it to a file.
 */
void indexBuild(const char* pageDirectory, const char* indexFilename)
{
    // Initialize a new index
    index_t* index = index_new(1000);
    if (index == NULL)
    {
        return;
    }

    for (int docID = 1; ; docID++)
    {
        // Formulate the filename for each webpage
        char filename[strlen(pageDirectory) + 20];
        sprintf(filename, "%s/%d", pageDirectory, docID);
        
        // Open the webpage file
        FILE* fp = fopen(filename, "r");

        // Break out of loop if file doesn't exist
        if (fp == NULL)
        {
            break;
        }
        
        // Load the webpage and process its content
        webpage_t* webpage = pagedir_load(fp);
        fclose(fp);
        if (webpage != NULL)
        {
            indexPage(index, webpage, docID);
            webpage_delete(webpage);
        }
    }

    // Save the completed index to a file and cleanup
    index_save(index, indexFilename);
    index_delete(index);
}

/*
 * indexPage: Extracts words from a webpage, normalizes them, 
 * and updates the index.
 *
 * Words shorter than 3 characters are ignored. For each valid word, 
 * its occurrence is noted with a document ID in the index.
 */
void indexPage(index_t* index, webpage_t* webpage, int docID)
{
    int pos = 0;
    char* word;
    while ((word = webpage_getNextWord(webpage, &pos)) != NULL)
    {
        // Ignore words that are too short
        if (strlen(word) < 3)
        {
            free(word);
            continue;
        }

        // Normalize the word for consistent indexing
        normalizeWord(word);
        
        // Search for the word in the index
        counters_t* ctrs = index_find(index, word);
        if (ctrs == NULL)
        {
            // If not found, create a new counters object and insert in index
            ctrs = counters_new();
            index_insert(index, word, ctrs);
        }

        // Update word's occurrence for the current document ID
        counters_add(ctrs, docID);
        free(word);
    }
}