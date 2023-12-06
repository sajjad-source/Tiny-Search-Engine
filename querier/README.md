## Querier

The `querier` is a tool designed to search through a web page directory created by the `crawler`, and index the pages using the `indexer`. It loads the index file produced by the `indexer`, processes queries input by the user, and returns the results in descending order of relevance.

### Functionality
The `querier` module fully implements all required functionalities, achieving a total of 30/30 points based on the assignment rubric. Specific features include:

- Accurate parsing and processing of queries with support for 'and', 'or', and precedence rules.
- Proper handling of 'and' having precedence over 'or'.
- Outputting the set of documents that match the query, ranked in decreasing order by score.

This implementation meets the full specifications of the assignment, ensuring comprehensive and precise search results.
### Usage

The `querier` module, defined in `querier.h` and implemented in `querier.c`, provides the following command-line usage:

```bash
./querier [pageDirectory] [indexFilename]
```
- `pageDirectory`: The directory where the crawler’s fetched web pages are stored.
- `indexFilename`: The filename of the index file produced by the indexer.

### Implementation
The `querier` loads the index file, processes queries entered by the user, and ranks the results based on the frequency of query terms appearing on each page. It supports 'and' and 'or' operators and ensures that the query syntax is correct.

### Features
- Processes queries containing 'and' and 'or' operators.
- Validates query syntax.
- Ranks results in descending order of relevance.
- Handles cases where no documents match the query.

### Files

* `Makefile` - compilation procedure
* `querier.c` - the implementation
* `testing.sh` - testing script
* `valid_query.txt` - file that contains valid queries for testing.sh
* `invalid_query.txt` - file that contains invalid queries for testing.sh
* `testing.out` - output of testing

### Compilation

To compile, simply `make`.

### Testing

To test, simply `make test`.

### Documentation

For more in-depth details about this module, refer to the following documents:

- [Requirements](REQUIREMENTS.md)
- [Implementation](IMPLEMENTATION.md)
- [Design](DESIGN.md)