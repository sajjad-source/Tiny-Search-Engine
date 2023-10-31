/*
 * querier.c    Sajjad C Kareem    October 24, 2023
 * 
 * Implements the search engine querier functionality.
 * 
 * This file contains functions necessary to interpret and execute search
 * queries on a given index produced by the TSE indexer. It supports
 * logical AND and OR operations, ranks the results based on the number
 * of matches, and outputs the results to the user.
 * 
 */

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "counters.h"
#include "file.h"
#include "webpage.h"
#include "word.h"
#include "pagedir.h"
#include "index.h"

/*
 * two_sets_t: A structure to store two sets of counters.
 *
 * Fields:
 * - ctr1: A pointer to the first set of counters.
 * - ctr2: A pointer to the second set of counters.
 */
typedef struct
{
    counters_t *ctr1, *ctr2;
} two_sets_t;

/*
 * doc_t: A structure to store document information.
 *
 * Fields:
 * - docID: An integer representing the document's ID.
 * - score: An integer representing the document's score.
 */
typedef struct
{
    int docID;
    int score;
} doc_t;

/*
 * doc_array_t: A structure to store an array of documents.
 *
 * Fields:
 * - array: An array of doc_t structures.
 * - index: An integer representing the current index in the array.
 */
typedef struct {
    doc_t *array;
    int index;
} doc_array_t;

bool validate_query(char** tokens, int numTokens);
char* read_query();
char* getURL(int docID, const char* pageDirectory);
char** tokenize_query(char* query, int* numTokens);
int compare_score(const void* score1, const void* score2);
void process_query(char* query, index_t* index, const char* pageDirectory);
void print_query(void* arg, const int key, const int count);
void free_tokens(char** tokens, int numTokens);
void intersect_counters(void* arg, const int key, const int count);
void union_counters(void* arg, const int key, const int count);
void copy_counter(void *arg, const int key, const int count);
void count_size(void* arg, const int key, const int count);
void sort_helper(void *arg, const int key, int count);
doc_t* sort_result(counters_t* result, int* result_size);
counters_t* counters_intersect(counters_t* ctrA, counters_t* ctrB);
counters_t* counters_union(counters_t* ctrA, counters_t* ctrB);
counters_t* counters_copy(counters_t *counters);

int main(int argc, char const *argv[])
{
    // Check for the correct number of command line arguments
    if (argc != 3)
    {
        printf("Usage: ./querier pageDirectory indexFilename\n");
        return 1;
    }

    const char *pageDirectory = argv[1];
    const char *indexFilename = argv[2];

    // Validate that the provided page directory was produced by the Crawler
    if (!pagedir_validate(pageDirectory))
    {
        printf("The pageDirectory %s is not a crawler produced directory\n", pageDirectory);
        return 2;
    }

    // Attempt to open the index file for reading
    FILE* fp = fopen(indexFilename, "r");
    if (fp == NULL)
    {
        printf("Could not open index file: %s\n", indexFilename);
        return 1;
    }

    index_t* index = index_load(fp);
    fclose(fp);

    if (index == NULL)
    {
        printf("Failed to load index from %s\n", indexFilename);
        return 3;
    }

    // Enter the main query processing loop
    char* query;
    while ((query = read_query()) != NULL)
    {
        process_query(query, index, pageDirectory);
        free(query);
    }

    // Cleanup
    index_delete(index);
    return 0;
}

/*
 * read_query: Read and preprocess a search query from the standard input.
 *
 * This function prompts the user to enter a query, reads the input line,
 * removes the trailing newline character, if any, and converts all characters
 * to lowercase. The cleaned query string is returned.
 *
 * Memory for the query string is dynamically allocated and must be freed by
 * the caller.
 *
 * Returns:
 * - char*: A pointer to the cleaned query string.
 */
char* read_query()
{
    printf("Query? ");
    char* query = NULL;
    size_t size = 0;
    ssize_t len = getline(&query, &size, stdin); // Read the query from the user, allocating memory as needed

    if (len == -1) { // Handle EOF or error
        free(query); // Cleanup
        return NULL;
    }

    if (len > 0 && query[len - 1] == '\n') {    // Check if the input string ends with a newline character and remove it
        query[len - 1] = '\0';
        len--;
    }

    for (int i = 0; i < len; i++) {
        query[i] = tolower(query[i]);           // Convert the query to lowercase for case-insensitive processing
    }

    return query;                               // Return the cleaned query string
}


/*
 * tokenize_query: Tokenize a query string into individual words.
 *
 * This function takes a string representing a search query, breaks it into
 * individual tokens based on spaces, and stores each token in an array of
 * strings. The function ensures that the tokens are properly formatted and
 * returns a pointer to the array.
 *
 * The number of tokens is stored in the variable pointed to by numTokens.
 * Memory for the array and the tokens is dynamically allocated and must be
 * freed by the caller.
 * 
 * Returns:
 * - char**: A pointer to the array of token strings.
 */

char** tokenize_query(char* query, int* numTokens)
{
    // Initialize tokens array and variables for tracking its size and capacity
    char** tokens = NULL;
    int capacity = 0;
    int size = 0;

    char* token = strtok(query, " ");                               // Tokenize the query string using space as a delimiter
    while (token != NULL)
    {
        if (size >= capacity)                                       // Check if the tokens array needs to be resized
        {
            capacity = capacity*2 + 1;
            tokens = realloc(tokens, capacity * sizeof(char*));     // Resize the tokens array
        }

        tokens[size++] = calloc(sizeof(char), strlen(token) + 1);   // Allocate memory for the current token and copy it to the tokens array
        strcpy(tokens[size-1], token);   

        token = strtok(NULL , " ");                                 // Get the next token
    }

    *numTokens = size;
    return tokens;                                                  // Return tokens array
}

/*
 * validate_query: Checks the syntax of a tokenized query for common errors.
 *
 * The function iterates through the array of token strings, checking for invalid characters
 * and incorrect usage of 'and'/'or' operators. If any syntax errors are found, an error message
 * is printed, the memory used by the tokens is freed, and the function returns false to indicate an error.
 * 
 * If the query is valid, the function returns true.
 */
bool validate_query(char** tokens, int numTokens)
{
    // Check for syntax errors
    for (int i = 0; i < numTokens; i++)
    {
        // Check for invalid characters
        for (int j = 0; tokens[i][j] != '\0'; j++)
        {
            if (!isalpha(tokens[i][j]))
            {
                printf("Error: bad character '%c' in query\n", tokens[i][j]);
                free_tokens(tokens, numTokens);
                return false;
            }
        }

        // Check for 'and' or 'or' in beginning or end
        if ((strcmp(tokens[i], "and") == 0 || strcmp(tokens[i], "or") == 0) && (i == 0 || i == numTokens - 1))
        {
            printf("Error: '%s' cannot be first or last\n", tokens[i]);
            free_tokens(tokens, numTokens);
            return false;
        }

        if (i > 0 && (strcmp(tokens[i],"and") == 0 || strcmp(tokens[i], "or") == 0) &&
            (strcmp(tokens[i - 1], "and") == 0 || strcmp(tokens[i - 1], "or") == 0)) 
            {
                printf("Error: '%s' and '%s' cannot be adjacent\n", tokens[i - 1], tokens[i]);
                free_tokens(tokens, numTokens);
                return false;
            }
    }
    return true;
}

/*
 * process_query: Processes a given query string, searching the index for matching documents.
 *
 * The function tokenizes the query, validates it, and then searches the index for documents
 * that match the query based on 'and'/'or' operators. The matching documents are then sorted
 * by score and printed.
 */
void process_query(char* query, index_t* index, const char* pageDirectory)
{
    int numTokens;
    char** tokens = tokenize_query(query, &numTokens);                  // Tokenize the query

    if (tokens == NULL)                                                 // Handle cases where tokenization fails or results in an empty query
    {
        printf("Error: failed to tokenize query\n");
        free_tokens(tokens, numTokens);
        return;
    }

    if (numTokens == 0)
    {
        printf("Error: empty query\n");
        free_tokens(tokens, numTokens);
        return;
    }

    if (!validate_query(tokens, numTokens))                            // Validate the query syntax and return if invalid
    {
        return;
    }

    counters_t* result = NULL;
    counters_t* temp = NULL;

    for (int i = 0; i < numTokens; i++)                                // Iterate through each token in the query
    {
        if (strcmp(tokens[i], "and") == 0)                             // Skip "and" tokens, as they don't directly affect the result
        {
            continue;
        } else if (strcmp(tokens[i], "or") == 0) {                     // If an "or" token is found, combine the current results and temp counters
            if (result == NULL)
            {
                result = temp;
                temp = NULL;
            } else if (temp != NULL) {
                counters_t* new_result = counters_union(result, temp);
                counters_delete(result);
                counters_delete(temp);
                result = new_result;
                temp = NULL;
            }
        } else {
            counters_t* word_counters = index_find(index, tokens[i]);
            if (word_counters != NULL)
            {
                if (temp == NULL)
                {
                    temp = counters_copy(word_counters);
                } else {
                    counters_t* new_temp = counters_intersect(temp, word_counters);
                    counters_delete(temp);
                    temp = new_temp;
                }
            }
        }
    }

    // Handle the case where the last token in the query was not an "or"
    if (result == NULL)
    {
        result = temp;
    } else if (temp != NULL)
    {
        counters_t* new_result = counters_union(result, temp);
        counters_delete(result);
        counters_delete(temp);
        result = new_result;
    }
    
    // If there are no results, inform the user and return
    if (result == NULL) 
    {
        printf("No documents match.\n");
        free_tokens(tokens, numTokens);
        return;
    }
    
    // Sort the results based on the document scores
    int size;
    doc_t* results = sort_result(result, &size);
    if (results == NULL && size != 0) 
    {
        counters_delete(result);
        free_tokens(tokens, numTokens);
        return;
    }

    if (size == 0) {
        printf("No documents match.\n");
    } else {
        // Print the sorted results
        printf("Matches %d documents (ranked):\n", size);
        for (int i = 0; i < size; i++) 
        {
            print_query((void*)pageDirectory, results[i].docID, results[i].score);
        }

        free(results);
    }

    // Cleanup
    counters_delete(result);
    free_tokens(tokens, numTokens);

    return;
}

/*
 * print_query: Prints the results of a query search, showing the score, document ID, and URL.
 *
 * The function takes in a page directory, document ID, and score as arguments.
 * It constructs the file path for the document, retrieves the URL from the file, and prints the results.
 * The function ensures that valid results with scores greater than 0 are printed.
 */
void print_query(void* arg, const int key, const int count) 
{
    if (count > 0) 
    {
        const char* pageDirectory = arg;
        char* url = getURL(key, pageDirectory);
        if (url != NULL) 
        {
            printf("score\t%d doc\t%d: %s\n", count, key, url);
            free(url);
        }
    }
}

/*
 * counters_intersect: Calculates the intersection of two counters sets.
 *
 * The function iterates through the first set of counters, and for each key,
 * it finds the minimum of the counts in the two sets and updates the result set with that minimum.
 * If a key is not present in the second set, the count for that key in the result set is set to 0.
 * The function returns a new counters set representing the intersection.
 */
counters_t* counters_intersect(counters_t* ctrA, counters_t* ctrB) 
{
    counters_t* result = counters_new();
    if (result != NULL) 
    {
        two_sets_t two_sets = {result, ctrB};
        counters_iterate(ctrA, &two_sets, intersect_counters);
    }
    return result;
}

/*
 * counters_union: Calculates the union of two counters sets.
 *
 * The function iterates through both sets of counters, summing the counts for each key
 * and updating the result set with the sum. The function returns a new counters set
 * representing the union.
 */
counters_t* counters_union(counters_t* ctrA, counters_t* ctrB) 
{
    counters_t* result = counters_new();
    if (result != NULL)
    {
        counters_iterate(ctrA, result, union_counters);
        counters_iterate(ctrB, result, union_counters);
    }
    return result;
}

/*
 * counters_copy: Creates a copy of a counters set.
 *
 * The function iterates through the input counters set, copying each key-count pair
 * to a new counters set. The function returns the new counters set, or NULL on error.
 */
counters_t* counters_copy(counters_t *counters) 
{
    if (counters == NULL) 
    {
        return NULL;
    }
    counters_t *new_counters = counters_new();
    if (new_counters == NULL) 
    {
        return NULL;
    }

    counters_iterate(counters, new_counters, copy_counter);
    return new_counters;
}

/*
 * intersect_counters: Helper function for counters_intersect, performs intersection on two counters.
 *
 * The function takes in a key and a count from the first set of counters, finds the corresponding
 * count in the second set of counters, and updates the result set with the minimum of the two counts.
 * If the key is not present in the second set, the count for that key in the result set is set to 0.
 */
void intersect_counters(void* arg, const int key, const int count)
{
    two_sets_t* two_sets = arg;
    int countB = counters_get(two_sets->ctr2, key);
    if (countB > 0)
    {
        counters_set(two_sets->ctr1, key, (count < countB) ? count : countB);
    } else {
        counters_set(two_sets->ctr1, key, 0);
    }
}

/*
 * union_counters: Helper function for counters_union, performs union on two counters.
 *
 * The function takes in a key and a count, finds the current count for that key in the result set,
 * and updates the result set with the sum of the two counts.
 */
void union_counters(void* arg, const int key, const int count)
{
    counters_t* result_counters = arg;
    int curr_word_count = counters_get(result_counters, key);
    counters_set(result_counters, key, curr_word_count + count);
}

/*
 * copy_counter: Helper function for counters_copy, copies a key-count pair to a new counters set.
 *
 * The function takes in a key and a count from the input counters set,
 * and sets the same key and count in the new counters set.
 */
void copy_counter(void *arg, const int key, const int count) 
{
    counters_t* new_counters = arg;
    counters_set(new_counters, key, count);
}

/*
 * compare_score: Compares two document scores for sorting.
 *
 * The function takes in two document structs, compares their scores,
 * and returns the difference. This function is meant to be used with qsort
 * to sort an array of documents in descending order of score.
 */
int compare_score(const void* score1, const void* score2)
{
    return ((doc_t*)score2)->score - ((doc_t*)score1)->score;
}

/*
 * free_tokens: Frees the memory used by an array of token strings.
 *
 * The function iterates through the array of token strings, freeing each string,
 * and then frees the array itself.
 */
void free_tokens(char** tokens, int numTokens)
{
    for (int i = 0; i < numTokens; i++) 
    {
        free(tokens[i]);
    }
    free(tokens);
}

/*
 * count_size: Helper function to count the number of positive counts in a counters set.
 *
 * The function takes in a key and a count, and increments the size counter if the count is greater than 0.
 */
void count_size(void* arg, const int key, const int count)
{
    if (count > 0)
    {
        int* size = arg;
        (*size)++;
    }
}

/*
 * sort_helper: Helper function for sorting results, populates an array with document structs.
 *
 * The function takes in a key and a count, creates a document struct with the key as docID and the
 * count as score, and adds the struct to the array. This function is meant to be used with counters_iterate
 * to populate an array with document structs from a counters set.
 */
void sort_helper(void *arg, const int key, int count)
{
    doc_array_t *data = (doc_array_t *)arg;
    if (count > 0) 
    {
        data->array[data->index].docID = key;
        data->array[data->index].score = count;
        data->index++;
    }
}

/*
 * getURL: Retrieves the URL of a document from its file in the page directory.
 *
 * The function constructs the file path of the document using the page directory and document ID,
 * opens the file, reads the URL from the first line of the file, and returns the URL as a string.
 * The function returns NULL on error and prints an error message.
 */
char* getURL(int docID, const char* pageDirectory)
{
    char filePath[100];
    sprintf(filePath, "%s/%d", pageDirectory, docID);

    FILE* file = fopen(filePath, "r");
    if (file == NULL) 
    {
        fprintf(stderr, "Failed to open file %s\n", filePath);
        return NULL;
    }

    char* url = NULL;
    size_t len = 0;
    if (getline(&url, &len, file) == -1) 
    {
        fprintf(stderr, "Failed to read URL from file %s\n", filePath);
        free(url);
        fclose(file);
        return NULL;
    }

    size_t url_len = strlen(url);
    if (url_len > 0 && url[url_len - 1] == '\n') 
    {
        url[url_len - 1] = '\0';
    }

    fclose(file);
    return url;
}

/*
 * sort_result: Sorts the results of a query based on score.
 *
 * The function takes in a counters set representing the results of a query,
 * counts the number of positive counts in the set, sorts the results based on score,
 * and returns an array of document structs representing the sorted results.
 * The function also sets the result_size output parameter to the number of results.
 */
doc_t* sort_result(counters_t* result, int* result_size)
{
    int size = 0;
    counters_iterate(result, &size, count_size);             // Count the size of the result sets

    *result_size = size;

    if (size == 0) {
        return NULL;
    }

    doc_t* results = malloc(size * sizeof(doc_t));          // Create a new results that will be the sorted version

    if (results == NULL) 
    {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    doc_array_t data = {results, 0};                        // Store an array of the results

    counters_iterate(result, &data, sort_helper);           // populates an array with document structs.
    qsort(results, size, sizeof(doc_t), compare_score);     // use quicksort to sort the results by score

    return results;                                         // return the sorted results
}