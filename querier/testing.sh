#!/bin/bash
#
# testing.sh - script to test the querier program
# Assumption - The files and directories used in the program are assumed to exist when tested.

echo "====================================================="
echo "Testing invalid arguments..."
echo "====================================================="
./querier
./querier ../crawler/data/letters-0
./querier ../crawler/data/letters-0 ../indexer/data/letters-0.index ../indexer/data/letters-0.index
./querier ../crawler/data/nonexistent ../indexer/data/letters-0.index
./querier ../crawler/data/letters-0 ../indexer/data/nonexistent/index

echo "====================================================="
echo "Creating a directory without .crawler for testing..."
echo "====================================================="
mkdir ../crawler/data/noncrawler_dir
./querier ../crawler/data/noncrawler_dir ../indexer/data/letters-0.index
rmdir ../crawler/data/noncrawler_dir

echo "====================================================="
echo "Testing with valid arguments and fuzzquery..."
echo "====================================================="
./fuzzquery ../indexer/data/letters-0.index 25 1234 | ./querier ../crawler/data/letters-0 ../indexer/data/letters-0.index
./fuzzquery ../indexer/data/letters-10.index 30 4321 | ./querier ../crawler/data/letters-10 ../indexer/data/letters-10.index
./fuzzquery ../indexer/data/toscrape-0.index 20 5678 | ./querier ../crawler/data/toscrape-0 ../indexer/data/toscrape-0.index
./fuzzquery ../indexer/data/toscrape-1.index 15 8765 | ./querier ../crawler/data/toscrape-1 ../indexer/data/toscrape-1.index
./fuzzquery ../indexer/data/toscrape-2.index 30 7580 | ./querier ../crawler/data/toscrape-1 ../indexer/data/toscrape-1.index
./fuzzquery ../indexer/data/wikipedia-0.index 10 7890 | ./querier ../crawler/data/wikipedia-0 ../indexer/data/wikipedia-0.index
./fuzzquery ../indexer/data/wikipedia-1.index 5 9870 | ./querier ../crawler/data/wikipedia-1 ../indexer/data/wikipedia-1.index

echo "====================================================="
echo "Testing valid queries from valid_query.txt..."
echo "====================================================="
./querier ../crawler/data/letters-10 ../indexer/data/letters-10.index < valid_query.txt
./querier ../crawler/data/toscrape-0 ../indexer/data/toscrape-0.index < valid_query.txt
./querier ../crawler/data/toscrape-1 ../indexer/data/toscrape-1.index < valid_query.txt
./querier ../crawler/data/wikipedia-0 ../indexer/data/wikipedia-0.index < valid_query.txt
./querier ../crawler/data/wikipedia-1 ../indexer/data/wikipedia-1.index < valid_query.txt

echo "====================================================="
echo "Testing invalid queries from invalid_query.txt..."
echo "====================================================="
./querier ../crawler/data/toscrape-1 ../indexer/data/toscrape-1.index < invalid_query.txt

echo "====================================================="
echo "Running Valgrind tests..."
echo "====================================================="
valgrind --leak-check=full --show-leak-kinds=all ./querier ../crawler/data/wikipedia-0 ../indexer/data/wikipedia-0.index < valid_query.txt

echo "Testing completed."
echo "====================================================="