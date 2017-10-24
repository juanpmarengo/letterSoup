#!/bin/bash
# Run script

if [ "$1" = "test" ]
then
    gcc ./src/test/letterSoupTest.c -o build/output && ./build/output
else
    gcc ./src/main/letterSoup.c -o build/output && ./build/output
fi