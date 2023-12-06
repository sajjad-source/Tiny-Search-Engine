# Tiny Search Engine

## Overview
Tiny Search Engine (TSE) is a minimalistic yet powerful search engine project comprised of three core components: a crawler, an indexer, and a querier. Each component is designed to perform a specific task in the process of searching and indexing web content, working together to provide an efficient search engine experience.

### Components
1. **Crawler**: The crawler is responsible for traversing web pages and collecting data. It starts from a seed URL and explores pages to a specified depth.
   - Detailed information and usage instructions can be found in the crawler [`README.md`](/crawler/README.md).

2. **Indexer**: This component processes the data collected by the crawler, building an index that correlates keywords with their occurrences in the web pages.
   - For more details, refer to the indexer [`README.md`](/indexer/README.md).

3. **Querier**: The querier handles user queries, searching the index created by the indexer to find relevant web page results.
   - Instructions and more information are available in the [`README.md`](/querier/README.md).

## Development Environment
Please note that Tiny Search Engine and its components were developed on a Linux-based system. This might impact compatibility and functionality when attempting to run or develop the project on other operating systems. 

---

For more detailed information about each component of the Tiny Search Engine, please refer to the respective README.md files in their directories.
