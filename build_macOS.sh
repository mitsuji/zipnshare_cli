#!/bin/sh

export CC_INCLUDE="-I /Users/mitsuji/apps/homebrew/Cellar/libmagic/5.40/include"
export MAGIC_LIB_FILE="/Users/mitsuji/apps/homebrew/Cellar/libmagic/5.40/lib/libmagic.a"

gcc -c -std=c++17 $CC_INCLUDE src/exception.cpp -o dest/exception.o
gcc -c -std=c++17 $CC_INCLUDE src/utf8_none.cpp -o dest/utf8.o
gcc -c -std=c++17 $CC_INCLUDE src/args.cpp -o dest/args.o
gcc -c -std=c++17 $CC_INCLUDE src/file_none.cpp -o dest/file.o
gcc -c -std=c++17 $CC_INCLUDE src/upload.cpp -o dest/upload.o
gcc -c -std=c++17 $CC_INCLUDE src/content_type_magic.cpp -o dest/content_type.o
gcc -c -std=c++17 $CC_INCLUDE src/main.cpp -o dest/main.o

gcc dest/exception.o dest/utf8.o dest/args.o dest/file.o dest/upload.o dest/content_type.o dest/main.o -o dest/zipnshare-cli -lstdc++ -lcurl $MAGIC_LIB_FILE -lbz2 -lz -llzma

