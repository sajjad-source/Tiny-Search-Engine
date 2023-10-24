# CS50 Lab 3 - Common Module
## CS50 Fall 2023

### Overview
The `common` directory contains shared utilities and functions for the CS50 tiny search engine project.
This code is designed to be used across the `crawler`, `indexer`, and `querier`.

### Functionality
- The `pagedir_init` function is designed to initialize a page directory, marking it as crawler-produced.
- The `pagedir_save` function serves the purpose of saving fetched webpages into a specified directory. Each saved page is indexed by its document ID. The contents of these files include the URL, depth, and HTML content of the web page.
- The `pagedir_validate` function examines a directory to determine if it looks like a crawler output directory
- The `pagedir_load` fucntion reads a file and extracts webpage data from it
- The `index` module provides functionality related to the creation, manipulation, and saving/loading of the word-document index.
- The `word` module contains utilities for handling and processing words before they are added to the index.

### Files
- `pagedir.h`: Header file with function declarations and documentation.
- `pagedir.c`: Implementation of the utilities mentioned.
- `index.h`: Header file with function declarations and documentation for the index.
- `index.c`: Implementation of the index.
- `word.h`: Header file with function declarations and documentation for word utilities.
- `word.c`: Implementation of word utilities.
- `Makefile`: Compilation instructions for the utilities in the common directory.

### Documentation

For more in-depth details, refer to the following documents:

- [Requirements](../crawler/REQUIREMENTS.md)
- [Implementation](../crawler/IMPLEMENTATION.md)
- [Design](../crawler/DESIGN.md)