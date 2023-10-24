/*
 * index.c - CS50 'index' module
 *
 * see index.h for more information.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "word.h"
#include "../libcs50/file.h"
#include "index.h"

/**************** local types ****************/
typedef struct index 
{
    hashtable_t* ht;
} index_t;

/**************** index_new() ****************/
/* see index.h for description */
index_t* index_new(int num_slots)
{
    if (num_slots < 0)
    {
        return NULL;
    }

    index_t* index = malloc(sizeof(index_t));
    if (index == NULL)
    {
        return NULL;
    }
    index->ht = hashtable_new(num_slots);
    if (index->ht == NULL)
    {
        free(index);
        return NULL;
    }
    return index;
}

/**************** index_insert() ****************/
/* see index.h for description */
bool index_insert(index_t* index, char* word, counters_t* ctrs)
{
    if (index == NULL || word == NULL || ctrs == NULL)
    {
        return false;
    }
    return hashtable_insert(index->ht, word, ctrs);
}

/**************** index_delete() ****************/
/* see index.h for description */
void index_delete(index_t* index)
{
    hashtable_delete(index->ht, item_delete);
    free(index);
}

/**************** index_find() ****************/
/* see index.h for description */
counters_t* index_find(index_t* index, char* word)
{
    if (index == NULL || word == NULL)
    {
        return NULL;
    }
    return hashtable_find(index->ht, word);
}

/**************** item_delete() ****************/
/* Helper function to delete items */
void item_delete(void* item)
{
    if (item != NULL)
    {
        counters_delete((counters_t*) item);
    }
}

/**************** index_save() ****************/
/* see index.h for description */
void index_save(index_t* index, const char* filename)
{
    FILE* fp = fopen(filename, "w");
    if (fp == NULL)
    {
        printf("Unable to open file");
        exit(1);
    }

    hashtable_iterate(index->ht, fp, index_print);
    fclose(fp);
}

/**************** index_load() ****************/
/* see index.h for description */
index_t* index_load(FILE* fp)
{
    index_t* index = index_new(500);
    char* word;
    int docID;
    int count;

    while ((word = file_readWord(fp)) != NULL)
    {
        counters_t* ctrs = counters_new();

        // Read docID and count pairs until we hit a newline
        while (fscanf(fp, " %d %d", &docID, &count) == 2)
        {
            counters_set(ctrs, docID, count);
            char c = fgetc(fp);
            if (c == '\n' || c == EOF)
            {
                break;
            } else {
                ungetc(c, fp);
            }
        }
        hashtable_insert(index->ht, word, ctrs);
        free(word);
    }
    return index;
}


/**************** index_print() ****************/
/* Helper function to print index items */
void index_print(void* arg, const char* key, void* item)
{
    if (arg == NULL || key == NULL || item == NULL)
    {
        return;
    }
    
    counters_t* ctrs = (counters_t*)item;
    FILE *fp = arg;
    fprintf(fp, "%s", key);
    counters_iterate(ctrs, fp, ctr_print);
    fprintf(fp, "\n");
}

/**************** ctr_print() ****************/
/* Helper function to print counters */
void ctr_print(void* arg, int key, int count)
{
    FILE *fp = arg;
    fprintf(fp, " %d %d", key, count);
}