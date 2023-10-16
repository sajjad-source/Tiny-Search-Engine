#!/bin/bash

# Testing incorrect arguments
echo "====================================================="
echo "Testing: Incorrect Arguments"
echo "====================================================="
./crawler # missing all arguments
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html # missing pageDirectory and maxDepth
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html data/letters-test # missing maxDepth
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html data/letters-test abc # invalid maxDepth
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html non_existent_directory 1 # non-existent directory
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html data/ 1 2 # more than 3 arguments

# Valgrind testing
echo "====================================================="
echo "Testing with valgrind: toscrape at depth 1"
echo "====================================================="
mkdir -p data/toscrape-1
valgrind --leak-check=full ./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html data/toscrape-1 1

echo "====================================================="
echo "Testing with valgrind: letters at depth 5"
mkdir -p data/letters-5
valgrind --leak-check=full ./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html data/letters-5 5

# CS50 websites testing
echo "====================================================="
echo "Testing letters at different depths"
echo "====================================================="
for depth in 0 1 2 10; do
    dir="data/letters-$depth"
    mkdir -p $dir
    ./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html $dir $depth
done

echo "====================================================="
echo "Testing toscrape at different depths"
echo "====================================================="
for depth in 0 1 2 3; do
    dir="data/toscrape-$depth"
    mkdir -p $dir
    ./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html $dir $depth
done

echo "====================================================="
echo "Testing wikipedia at different depths"
echo "====================================================="
for depth in 0 1 2; do
    dir="data/wikipedia-$depth"
    mkdir -p $dir
    ./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html $dir $depth
done
echo "====================================================="

echo "Finished testing"