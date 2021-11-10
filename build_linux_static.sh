#!/bin/sh

# [TODO] To make it portable if possible
# https://blog.gibson.sh/2017/11/26/creating-portable-linux-binaries/#how-and-why-libcurl-uses-symbol-versioning

gcc -DCURL_STATICLIB -DNGHTTP2_STATICLIB -c -std=c++17 src/exception.cpp -o dest/exception.o
gcc -DCURL_STATICLIB -DNGHTTP2_STATICLIB -c -std=c++17 src/utf8_none.cpp -o dest/utf8.o
gcc -DCURL_STATICLIB -DNGHTTP2_STATICLIB -c -std=c++17 src/args.cpp -o dest/args.o
gcc -DCURL_STATICLIB -DNGHTTP2_STATICLIB -c -std=c++17 src/file_none.cpp -o dest/file.o
gcc -DCURL_STATICLIB -DNGHTTP2_STATICLIB -c -std=c++17 src/upload.cpp -o dest/upload.o
gcc -DCURL_STATICLIB -DNGHTTP2_STATICLIB -c -std=c++17 src/content_type_magic.cpp -o dest/content_type.o
gcc -DCURL_STATICLIB -DNGHTTP2_STATICLIB -c -std=c++17 src/main.cpp -o dest/main.o

gcc dest/exception.o dest/utf8.o dest/args.o dest/file.o dest/upload.o dest/content_type.o dest/main.o -o dest/zipnshare-cli -static -static-libgcc -l:libstdc++.a -l:libmagic.a -l:libcurl.a -l:libz.a -l:libbz2.a -l:libnghttp2.a -l:libmbedtls.a -l:libmbedcrypto.a -l:libmbedx509.a

