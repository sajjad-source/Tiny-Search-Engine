# CS50 Lab 5
## CS50 Fall 2023

### Indexer

The `indexer` is a tool designed to process documents produced by the `crawler`, building an index that maps from words to documents. This index serves as the foundation for the search functionality facilitated by the `querier`.

### Usage

The `indexer` module, defined in `indexer.h` and implemented in `indexer.c`, offers the following command-line usage:

```bash
./indexer [pageDirectory] [indexFilename]
```
- pageDirectory: The directory where the crawler stored fetched web pages.
- indexFilename: The file where the indexer writes the index.

### Implementation
The `indexer` scans each document in the `pageDirectory`, tokenizing the content into words and updating the `index` structure. Each word points to one or more documents in which it appears.

The `indexer` gracefully handles various error scenarios, such as invalid arguments, missing directories, or indexing failures.

### Files

* `Makefile` - compilation procedure
* `indexer.c` - the implementation
* `indextest.c` - testing index
* `testing.sh` - testing script
* `testing.out` - output of testing

### Testing

To test, simply `make test`.

### Documentation

For more in-depth details about this module, refer to the following documents:

- [Requirements](REQUIREMENTS.md)
- [Implementation](IMPLEMENTATION.md)
- [Design](DESIGN.md)
