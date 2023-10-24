#!/bin/bash

# Set the base directory for crawler data
CRAWLER_DIR="../crawler/data"
# Set the base directory for indexer output
mkdir -p data
INDEXER_DIR="data/"
# Path name for indexcmp
INDEXCMP="~/cs50-dev/shared/students_files/tse/indexcmp"

# Testing incorrect arguments
echo "====================================================="
echo "Testing: Incorrect Arguments"
echo "====================================================="
./indexer                                                                   # missing all arguments
./indexer $CRAWLER_DIR/letters-1                                            # missing indexFilename
./indexer non_existent_directory $INDEXER_DIR/letters-1.index               # non-existent directory
./indexer $CRAWLER_DIR/letters-1 $INDEXER_DIR/letters-1.index extra_arg     # more than 2 arguments
./indexer $CRAWLER_DIR/nonexistent $INDEXER_DIR/letters-1.index             # invalid pageDirectory (not a crawler directory)
./indexer $CRAWLER_DIR/letters-1 non_existent_path/index                    # invalid indexFile (non-existent path)

# Testing read only
chmod -w $INDEXER_DIR
echo " Trying to write to a read only directory..."
./indexer $CRAWLER_DIR/letters-1 $INDEXER_DIR/letters-1.index
chmod +w $INDEXER_DIR

chmod -w $INDEXER_DIR/letters-1.index
echo "Trying to overwrite a read-only file..."
./indexer $CRAWLER_DIR/letters-1 $INDEXER_DIR/letters-1.index
chmod +w $INDEXER_DIR/letters-1.index

# Valgrind testing
echo "====================================================="
echo "Testing with valgrind: letters at depth 5"
echo "====================================================="
valgrind --leak-check=full ./indexer $CRAWLER_DIR/letters-5 $INDEXER_DIR/letters-5.index

echo "====================================================="
echo "Testing with valgrind: toscrape at depth 1"
echo "====================================================="
valgrind --leak-check=full ./indexer $CRAWLER_DIR/toscrape-1 $INDEXER_DIR/toscrape-1.index

# CS50 websites testing
echo "====================================================="
echo "Testing letters at different depths"
echo "====================================================="
for depth in 0 1 2 5 10; do
    crawler_dir="$CRAWLER_DIR/letters-$depth"
    indexer_file="$INDEXER_DIR/letters-$depth.index"
    ./indexer $crawler_dir $indexer_file
done

echo "====================================================="
echo "Testing toscrape at different depths"
echo "====================================================="
for depth in 0 1 2 3; do
    crawler_dir="$CRAWLER_DIR/toscrape-$depth"
    indexer_file="$INDEXER_DIR/toscrape-$depth.index"
    ./indexer $crawler_dir $indexer_file
done

echo "====================================================="
echo "Testing wikipedia at different depths"
echo "====================================================="
for depth in 0 1 2; do
    crawler_dir="$CRAWLER_DIR/wikipedia-$depth"
    indexer_file="$INDEXER_DIR/wikipedia-$depth.index"
    ./indexer $crawler_dir $indexer_file
done

echo "====================================================="
echo "Testing indextest"
echo "====================================================="
for depth in 0 1 2 5 10; do
    indexer_file="$INDEXER_DIR/letters-$depth.index"
    indextest_file="$INDEXER_DIR/letters-$depth.indextest"
    
    ./indextest $indexer_file $indextest_file
    echo "Comparing $indexer_file and $indextest_file with indexcmp"
    ~/cs50-dev/shared/tse/indexcmp $indexer_file $indextest_file
done

for depth in 0 1 2 3; do
    indexer_file="$INDEXER_DIR/toscrape-$depth.index"
    indextest_file="$INDEXER_DIR/toscrape-$depth.indextest"
    
    ./indextest $indexer_file $indextest_file
    echo "Comparing $indexer_file and $indextest_file with indexcmp"
    ~/cs50-dev/shared/tse/indexcmp $indexer_file $indextest_file
done

for depth in 0 1 2; do
    indexer_file="$INDEXER_DIR/wikipedia-$depth.index"
    indextest_file="$INDEXER_DIR/wikipedia-$depth.indextest"
    
    ./indextest $indexer_file $indextest_file
    echo "Comparing $indexer_file and $indextest_file with indexcmp"
    ~/cs50-dev/shared/tse/indexcmp $indexer_file $indextest_file
done

echo "====================================================="

echo "Finished testing"