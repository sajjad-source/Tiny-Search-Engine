/*
 * crawler.c    Sajjad C Kareem    October 10, 2023
 *
 * This file contains the definitions for a web crawler. 
 * Functions include:
 *     - parseArgs: To validate and parse the command-line arguments.
 *     - crawl: Crawling websites up to a specified depth.
 *     - pageScan: Scan a page for URLs and handle them.
 *     - strdup: Duplicate a string.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "webpage.h"
#include "set.h"
#include "hashtable.h"
#include "bag.h"
#include "pagedir.h"
#include <string.h>

/* Function declarations */
void parseArgs(int argc, char* argv[], char** seedURL, char** pageDirectory, int* maxDepth);
void crawl(char* seedURL, char* pageDirectory, int maxDepth);
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen);
char* strdup(char* str);

int main(int argc, char *argv[])
{
    char* seedURL;
    char* pageDirectory;
    int maxDepth;

    // Parsing arguments for crawler
    parseArgs(argc, argv, &seedURL, &pageDirectory, &maxDepth);

    // Begin crawling with given parameters
    crawl(seedURL, pageDirectory, maxDepth);

    free(seedURL);
    return 0;
}

/*
 * parseArgs: Validates and extracts the required arguments from command line.
 * for seedURL, normalize the URL and validate it is an internal URL
 * for pageDirectory, call pagedir_init()
 * for maxDepth, ensure it is an integer in specified range
 * if any trouble is found, print an error to stderr and exit non-zero.
 */
void parseArgs(int argc, char* argv[], char** seedURL, char** pageDirectory, int* maxDepth)
{
    if (argc != 4)                                                      // Checking number of arguments
        {
            printf("Usage: ./crawler seedURL pageDirectory maxDepth\n");
            exit(1);
        }

        char* normalizedURL = normalizeURL(argv[1]);                    // Normalizing seedURL
        if (normalizedURL == NULL)
        {
            printf("Error in normalizing seedURL\n");
            exit(1);
        }

        if (!isInternalURL(normalizedURL))                              // Check if URL is internal
        {
            printf("Error: seedURL is not internal\n");
            free(normalizedURL);
            exit(1);
        }
        *seedURL = normalizedURL;

        *pageDirectory = argv[2];
        if (!pagedir_init(*pageDirectory))                              // Initialize page directory
        {
            printf("Error initializing pageDirectory\n");
            free(normalizedURL);
            exit(1);
        }

        *maxDepth = atoi(argv[3]);                                      // Extract and check max depth
        if (*maxDepth < 0)
        {
            printf("maxDepth should be between 0 and 10\n");
            free(normalizedURL);
            exit(1);
        }
}

/*
 * crawl: Initiates the web crawling process up to the specified maxDepth.
 *
 * It maintains two data structures - a bag for pages to be crawled and a 
 * hashtable for pages already seen to avoid repetition.
 */
void crawl(char* seedURL, char* pageDirectory, int maxDepth)
{
    /* Hashtabe and Bag Initialization */
    hashtable_t* pagesSeen = hashtable_new(100);
    bag_t* pagesToCrawl = bag_new();

    char* wpSeedURL = strdup(seedURL);                          // Duplicate seedURL for webpage creation

    /* seed page initializtion */
    webpage_t* seedPage = webpage_new(wpSeedURL, 0, NULL);
    if (!seedPage) 
    {
        fprintf(stderr, "Failed to initialize seedPage.\n");
        return;
    }
    
    char* hashURL = strdup(seedURL);                           // Duplicate seedURL for hashtable insertion

    if (!hashtable_insert(pagesSeen, hashURL, ""))             // Add seedURL to hashtable
    {
        free(hashURL);
    }

    bag_insert(pagesToCrawl, seedPage);                        // Add seed page to bag for crawling

    /* Begin crawling */
    int docID = 1;                                          
    webpage_t* curr_page;
    while ((curr_page = bag_extract(pagesToCrawl)) != NULL)
    {
        if (webpage_fetch(curr_page))
        {
            printf("%d  Fetched: %s\n", webpage_getDepth(curr_page), webpage_getURL(curr_page));

            pagedir_save(curr_page, pageDirectory, docID);
            docID++;

            /* Scan page for URLs if not exceeded depth */
            if (webpage_getDepth(curr_page) < maxDepth)
            {
                pageScan(curr_page, pagesToCrawl, pagesSeen);
            }
        }
        webpage_delete(curr_page);
    }

    /* Cleanup */
    free(hashURL);
    hashtable_delete(pagesSeen, NULL);
    bag_delete(pagesToCrawl, NULL);
}


/*
 * pageScan: Scans the given webpage for URLs.
 *
 * For each URL, it normalizes the URL, checks if it's internal,
 * and if the URL hasn't been seen before, it adds it to the 
 * pages to be crawled.
 */
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen)
{
    int pos = 0;
    char* nextURL;
    int depth = webpage_getDepth(page);

    printf("%d  Scanning: %s\n", depth, webpage_getURL(page));
    while ((nextURL = webpage_getNextURL(page, &pos)) != NULL)
    {
        printf("%d  Found: %s\n", depth, nextURL);
        
        /* Normalize and check if URL is internal */
        char* normalizedURL = normalizeURL(nextURL);
        if (normalizedURL != NULL && isInternalURL(normalizedURL))
        {
            /* Insert in hashtable and bag */
            char* hashURL = strdup(normalizedURL);
            if (hashtable_insert(pagesSeen, hashURL, ""))
            {
                printf("%d  Added: %s\n", depth, normalizedURL);

                char* wpURL = strdup(normalizedURL);
                webpage_t* webpage = webpage_new(wpURL, depth + 1, NULL);

                if (webpage)
                {
                    bag_insert(pagesToCrawl, webpage);
                } else {
                    free(wpURL);
                }
            } else {
                printf("%d  IgnDupl: %s\n", depth, normalizedURL);
            }
            free(hashURL);
        } else {
            printf("%d  IgnExtrn: %s\n", depth, normalizedURL);
        }
        free(nextURL);
        free(normalizedURL);
    }
}

/* 
 * strdup: Creates a duplicate of the given string. 
 * 
 * Allocates memory for the duplicate, and copies 
 * the string into it.
 */
char* strdup(char* str)
{
    char* dup = malloc(strlen(str) + 1);

    if (dup != NULL)
       strcpy(dup, str);

    return dup;
}