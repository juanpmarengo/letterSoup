#!/bin/bash
# Run script

if [ ! -d "build" ]; then
    mkdir build
fi

if [ "$1" = "test" ]; then
	gcc -o build/test src/main/letterSoup.c src/test/test.c && ./build/test
else
	gcc -o build/main src/main/letterSoup.c src/main/main.c && ./build/main
fi