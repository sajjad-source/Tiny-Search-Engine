/*
 * word.c    Sajjad C Kareem    October 17, 2023
 *
 * This file contains the definition of a function that normalizes
 * a word. The normalization process ensures that the word is in 
 * lowercase and is composed only of alphabetical characters.
 */

#include "word.h"

/* See word.h for description*/
void normalizeWord(char* word)
{
   
    if (word != NULL)                            // Ensure word is not NULL
    {
        for (char* c = word; *c != '\0'; c++)
        {
            if (isalpha(*c))                     // Check if character is alphabetical 
            {
                *c = tolower(*c);                // Convert character to lowercase
            }
        }
    }
}