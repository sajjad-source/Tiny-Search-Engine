# CS50 Lab 3
## CS50 Fall 2023

### Crawler

The `crawler` is a tool designed to traverse webpages, fetching content starting from a specified seed URL up to a defined depth. 
The retrieved web pages are stored locally in a structured directory.

### Usage

The `crawler` module, defined in `crawler.h` and implemented in `crawler.c`, provides the following command-line usage:

```bash
./crawler [seedURL] [pageDirectory] [maxDepth]
```
- `seedURL`: The starting URL for the crawler.
- `pageDirectory`: The directory where fetched web pages are stored.
- `maxDepth`:  Maximum depth to which the crawler should go. Depth `0` means only the seed URL is fetched.

### Implementation
The `crawler` starts from the `seedURL` and explores internal links to the specified `maxDepth`. 
The fetched content is stored in the `pageDirectory` in a structured manner. 
Duplicate URLs or those outside the domain are ignored to avoid redundancy.

The crawler `handles` different error scenarios, such as invalid arguments, unreachable URLs, or fetching failures.

### Files

* `Makefile` - compilation procedure
* `crawler.h` - the interface
* `crawler.c` - the implementation
* `testing.sh` - testing script

### Compilation

To compile, simply `make`.

### Testing

To test, simply `make test`.

### Documentation

For more in-depth details about this module, refer to the following documents:

- [Requirements](REQUIREMENTS.md)
- [Implementation](IMPLEMENTATION.md)
- [Design](DESIGN.md)