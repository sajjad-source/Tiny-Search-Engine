# CS50 TSE Indexer
## Implementation Spec

In this document we reference the [Requirements Specification](REQUIREMENTS.md) and [Design Specification](DESIGN.md) and focus on the implementation-specific decisions.

-  Data structures
-  Control flow: pseudo code for overall flow, and for each of the functions
-  Detailed function prototypes and their parameters
-  Error handling and recovery
-  Testing plan

## Data structures 

We utilize a primary data structure called the `index`, which is a hashtable where each key is a word and the associated item is a counters set. 
Each counter in the set corresponds to a document in which the word appears, and the count is the frequency of the word in that document.

## Control flow

The Indexer is primarily housed within two files: `index.c`, which provides the core functionality for handling the index, 
and `indexer.c`, where the index-building process is executed.

## indexer

### main
The `main` function simply calls `indexBuild` then exits zero.

### indexBuild
This function, located in `indexer.c`, orchestrates the process of building the index from web pages stored in a given directory.
```
	creates a new 'index' object
      loops over document ID numbers, counting from 1
        loads a webpage from the document file 'pageDirectory/id'
        if successful, 
          passes the webpage and docID to indexPage
```

### indexPage
This functoin, located in `indexer.c` processes each word in a webpage and updates the index.
```
    steps through each word of the webpage,
        skips trivial words (less than length 3),
        normalizes the word (converts to lower case),
        looks up the word in the index,
            adding the word to the index if needed
        increments the count of occurrences of this word in this docID
```

## indextest

### main
The `main` function acts as the entry point. It verifies the correct number of command-line arguments, extracts filenames, and then invokes the indextest function.

### indextest
`indextest` loads an index from an existing file and saves it to a new file. The primary steps are:

- Open the old index file for reading.
- Load the index from this file.
- Save the loaded index to a new file.
- Cleanup resources.

## Other modules

### index.h
Function prototypes:
```c
index_t* index_new(int num_slots);
bool index_insert(index_t* index, char* word, counters_t* ctrs);
void index_delete(index_t* index);
counters_t* index_find(index_t* index, char* word);
bool index_save(index_t* index, const char* filename);
index_t* index_load(FILE* fp);
void index_print(void* arg, const char* key, void* item);
void item_delete(void* item);
void ctr_print(void* arg, int key, int count);

```
For more descriptions, see the [header file](../common/index.h).

### word.h
Function protypes:
```c
void normalizeWord(char* word);
```
For more descriptions, see the [header file](../common/word.h).

### pagedir.h
Function protypes:
```c
bool pagedir_validate(const char* pageDirectory);
webpage_t* pagedir_load(FILE* fp);
```
For more descriptions, see the [header file](../common/pagedir.h).

## Error handling and recovery
All the command-line parameters are rigorously checked before any data structures are allocated or work begins; problems result in a message printed to stderr and a non-zero exit status.

Any erroneous conditions, such as an inability to read from the page directory or save to the index file, result in appropriate error messages being printed to stderr.

## Integration/System Testing for Indexer

We utilize a script named `testing.sh` to systematically test our `indexer` and `indextest`.

### 1. Erroneous Invocations
Initially, the script addresses a series of erroneous invocations. This ensures that the program can handle each possible user mistake gracefully.

### 2. Valgrind Testing
To verify the program's memory integrity, a Valgrind test is conducted on a medium-sized dataset, specifically `toscrape` at depth 1.

### 3. CS50 Websites Testing
The indexer's functionality is rigorously tested on three distinct CS50 web datasets:
- `letters` at depths 0, 1, 2, 5, 10 
- `toscrape` at depths 0, 1, 2, 3 
- `wikipedia` at depths 0, 1, 2

### 4. Indextest Verification
Post-indexing, the `indextest` is employed to compare the index results produced by the `indexer`. We leverage the `indexcmp` tool to ensure the indices' consistency and reliability.

To run `testing.sh`
```bash
make test
```
