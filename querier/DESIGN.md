# TSE Querier

## Design Spec

This document outlines the design choices and considerations for the `querier` module, focusing on its functionalities, data structures, and algorithms. It will cover various aspects such as user interaction, functional decomposition, data flow, and testing.

We reference the [Requirements Specification](REQUIREMENTS.md) and focus on the implementation-independent design decisions.

- User interface
- Inputs and outputs
- Functional decomposition into modules
- Pseudo code (plain English-like language) for logic/algorithmic flow
- Major data structures
- Testing plan

### User interface

The `querier` operates via a command-line interface and follows the usage:

```bash
./querier [pageDirectory] [indexFilename]
```

- `pageDirectory`: A directory with files produced by the Crawler.
- `indexFilename`: A file produced by the Indexer, containing the index data.

The user interacts with the querier by inputting queries into stdin, and the results are displayed on `stdout`. If an input is taken from a terminal, the querier will display prompts.

### Inputs and outputs

**Input**:
- `Initialization`: The querier takes two command-line arguments: the pageDirectory and indexFilename.
- `Runtime`: Queries are read from stdin, processed, and the results are displayed.

**Output**:
- `Query results`: For each valid query, the querier outputs the document set that matches the query, sorted in decreasing order by score.
- `Error messages`: Any errors or invalid queries are reported to stderr.

### Functional decomposition into modules

In the querier program, functionalities are split into various functions and structures for modular and efficient design. 
Here's an overview:

1. Data Structures:
    - `two_sets_t`: Holds two sets of counters, useful for operations involving two sets.
    - `doc_t`: Represents a document, storing its ID and score.
    - `doc_array_t`: Manages an array of documents, including a counter for indexing.
2. Query Processing and Validation:
    - `validate_query`: Ensures that the user's query follows the acceptable syntax and structure, returning a boolean value indicating validity.
    - `tokenize_query`: Converts the user's query string into an array of individual tokens.
    - `print_query`: Outputs the final query results to the user.
    - `free_tokens`: Releases memory allocated for query tokens.
3. Search and Results Handling
    - `process_query`: Main function to handle the processing of a query. It involves searching the index and managing results.
    - `sort_result`: Sorts the documents based on their scores to prepare for final output.
    - `sort_helper`: Helper function for populating an array of documents
    - `compare_score`: Comparator function used for sorting documents based on their scores.
4. Counter Set Operations
    - `intersect_counters`, `union_counters`, and `counters_copy`: Perform set operations on counters.
    - `count_size`, `copy_counter`: Assist in various tasks related to counters.


### Pseudo code for logic/algorithmic flow

The main flow of `querier.c` is:
```plaintext
Start
Parse command-line arguments
Load the index from index file
While there are queries from stdin
    Get a query
    Process the query
    Print results
End While
Clean up and exit
```

### Major data structures

In the querier program, two major data structures play a crucial role in managing and manipulating the information: `index` and `counters`

- Index: A data structure to store and quickly retrieve words and their occurrences in documents.
- Counters: Used to store a collection of (key, count) pairs, where keys are unique identifiers (such as document IDs) and counts are associated values (such as word frequencies).

### Testing plan

1. Input Validation: Test with invalid arguments and inputs to ensure robust error handling.
2. Query Processing: Test with a variety of queries, including edge cases, to ensure accurate and efficient processing.
3. Memory Management: Use tools like Valgrind to ensure there are no memory leaks or errors.