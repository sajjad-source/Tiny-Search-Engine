#ifndef __WORD_H
#define __WORD_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/*
 * normalizeWord: Converts all characters of a word to lowercase.
 *
 * It iterates through each character of the provided word and checks 
 * if it's an alphabetical character. If so, it converts it to lowercase.
 *
 * @param word: Pointer to the word string that needs normalization.
 */
void normalizeWord(char* word);

#endif //__WORD_H