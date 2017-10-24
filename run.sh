#!/bin/bash
# Run script

if [ ! -d "build" ]; then
    mkdir build
fi

if [ "$1" = "test" ]; then
    gcc ./src/test/letterSoupTest.c -o build/output && ./build/output
else
    gcc ./src/main/letterSoup.c -o build/output && ./build/output
fi