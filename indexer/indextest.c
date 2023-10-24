/*
 * indextest.c     Sajjad C Kareem    October 20, 2023
 *
 * This file provides a way to test the functionality of the
 * index saving and loading functions. It loads an index from
 * a file, then saves it to another file, allowing for 
 * comparison between the two for testing purposes.
 */

#include <stdio.h>
#include <stdlib.h>
#include "../common/index.h"

// Function prototypes
void indextest(const char* oldIndexFilename, const char* newIndexFilename);

int main(int argc, char const *argv[])
{
    // Ensure proper number of command line arguments
    if (argc != 3)
    {
        printf("Usage: /indextest oldIndexFilename newIndexFilename\n");
        return 1;
    }

    // Extract filenames from command line arguments
    const char* oldIndexFilename = argv[1];
    const char* newIndexFilename = argv[2];

    // Load index from old file and save to new file
    indextest(oldIndexFilename, newIndexFilename);

    return 0;
}

/*
 * indextest: Loads an index from the given old file and
 * then saves it to the specified new file.
 */
void indextest(const char* oldIndexFilename, const char* newIndexFilename)
{
    // Open the old index file for reading
    FILE* fp_oldIndex = fopen(oldIndexFilename, "r");
    if (fp_oldIndex == NULL)
    {
        printf("Unable to open old index file for reading\n");
        exit(1);
    }

    // Load the index from the file
    index_t* index = index_load(fp_oldIndex);
    fclose(fp_oldIndex);

    // Ensure index was loaded successfully
    if (index == NULL)
    {
        printf("Error loading index\n");
        exit(1);
    }

    // Save the index to the new file
    index_save(index, newIndexFilename);

    // Cleanup
    index_delete(index);
}
