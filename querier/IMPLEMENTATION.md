# TSE Querier
## Implementation Spec

In this document we reference the [Requirements Specification](REQUIREMENTS.md) and [Design Specification](DESIGN.md) and focus on the implementation-specific decisions.

-  Data structures
-  Control flow: pseudo code for overall flow, and for each of the functions
-  Detailed function prototypes and their parameters
-  Error handling and recovery
-  Testing plan

## Data structures 

The Querier utilizes various data structures:

- `index_t`: A hashtable storing the inverted index, mapping from words to document IDs and counts.
- `counters_t`: A set of counters that hold the document IDs and the number of occurrences for each word.
- `doc_t`: A struct to hold document ID and score pairs, used for sorting and displaying the final results.

## Control flow

Querier is implemented only through `querier.c`, with testing given by `fuzzquery.c`.

### main

`main` function serves to initialize the necessary data structures, parse the command-line arguments,
load the index from the file, and enter a loop to process queries until termination.

### process_query

Given a query, this function tokenizes it, validates the syntax, and performs the search to find matching documents. It handles logical AND and OR operations as specified.

```plaintext
Initialize `result` and `temp` counters to NULL.
Loop through each token in the query.
    If "and", continue to the next token.
    If "or", perform union operation on `result` and `temp`, and reset `temp`.
    For normal tokens, find the postings list in the index and intersect with `temp`.
Perform final union or intersection operation if necessary.
If `result` is not NULL, sort and display the results; otherwise, print "No documents match."             
```

## Function Prototypes
Detailed descriptions are provided in the `querier.c` file. Below are some of the key function prototypes:

```c
bool validate_query(char** tokens, int numTokens);
char* read_query();
char* getURL(int docID, const char* pageDirectory);
char** tokenize_query(char* query, int* numTokens);
int compare_score(const void* score1, const void* score2);
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
```

## Error handling and recovery

The Querier is designed to be robust against invalid input and errors:

- Input validation is performed on command-line arguments.
- Query syntax is validated before processing.
- Error messages are provided for invalid inputs and queries.

Memory allocations are checked, and the program exits gracefully in case of failures, ensuring all allocated memory is freed.

## Integration/System Testing for Querier

We use a script named `testing.sh` to systematically test our `querier`, ensuring its functionality and robustness in various scenarios.

### 1. Erroneous Invocations
Initially, the script addresses a series of erroneous invocations. This ensures that the program can handle each possible user mistake gracefully.

### 2. Valgrind Testing
To verify the program's memory integrity, a Valgrind test is conducted on a medium-sized dataset, specifically targeting potential memory leaks and issues during the querying process.

### 3. Fuzzquery Testing
The `fuzzquery` tool is used to generate a series of random queries, which are then fed to the querier to test its robustness and error-handling capabilities under unpredictable conditions.

To run `testing.sh`
```bash
make test
```
