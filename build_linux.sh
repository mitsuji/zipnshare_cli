#!/bin/sh

gcc -c -std=c++17 src/exception.cpp -o dest/exception.o
gcc -c -std=c++17 src/utf8_none.cpp -o dest/utf8.o
gcc -c -std=c++17 src/args.cpp -o dest/args.o
gcc -c -std=c++17 src/file_none.cpp -o dest/file.o
gcc -c -std=c++17 src/upload.cpp -o dest/upload.o
gcc -c -std=c++17 src/content_type_magic.cpp -o dest/content_type.o
gcc -c -std=c++17 src/main.cpp -o dest/main.o

gcc dest/exception.o dest/utf8.o dest/args.o dest/file.o dest/upload.o dest/content_type.o dest/main.o -o dest/zipnshare-cli -lstdc++ -lcurl -lmagic
