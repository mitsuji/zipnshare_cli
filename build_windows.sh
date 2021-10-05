#!/bin/sh

gcc -DCURL_STATICLIB -DNGHTTP2_STATICLIB -c -std=c++17 src/exception.cpp -o dest/exception.o
gcc -DCURL_STATICLIB -DNGHTTP2_STATICLIB -c -std=c++17 src/utf8_win32.cpp -o dest/utf8.o
gcc -DCURL_STATICLIB -DNGHTTP2_STATICLIB -c -std=c++17 src/args.cpp -o dest/args.o
gcc -DCURL_STATICLIB -DNGHTTP2_STATICLIB -c -std=c++17 src/file_win32.cpp -o dest/file.o
gcc -DCURL_STATICLIB -DNGHTTP2_STATICLIB -c -std=c++17 src/upload.cpp -o dest/upload.o
gcc -DCURL_STATICLIB -DNGHTTP2_STATICLIB -c -std=c++17 src/content_type_win32.cpp -o dest/content_type.o
gcc -DCURL_STATICLIB -DNGHTTP2_STATICLIB -c -std=c++17 src/main.cpp -o dest/main.o

gcc dest/exception.o dest/utf8.o dest/args.o dest/file.o dest/upload.o dest/content_type.o dest/main.o -o dest/zipnshare-cli -static -lstdc++ -lshlwapi -lcurl -lnghttp2 -lidn2 -lssh2 -lpsl -ladvapi32 -lcrypt32 -lssl -lcrypto -lssl -lcrypto -lgdi32 -lwldap32 -lzstd -lbrotlidec -lz -lws2_32 -lbrotlicommon -liconv -lunistring



