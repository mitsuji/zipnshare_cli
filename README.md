# zipnshare_cli

This is a command line client for [zipnshare](https://github.com/mitsuji/zipnshare) file sharing system.

## How to use
```
# upload all files in current directory
$ zipnshare-cli

# upload all files in current directory without confirmation
$ zipnshare-cli -y

# upload file foo.txt specified by relative path
$ zipnshare-cli foo.txt

# upload file foo.txt specified by absolute path
$ zipnshare-cli /home/someone/foo.txt

# upload some files
$ zipnshare-cli foo1.txt foo2.txt /home/someone/foo.txt

# upload all files in directory /home/someone/somedir 
$ zipnshare-cli --dir /home/someone/somedir

# upload file foo.txt in directory /home/someone/somedir 
$ zipnshare-cli --dir /home/someone/somedir foo.txt
```	


## How to build

### Linux (for ex. Ubuntu 20.04 LTS)
```
$ sudo apt install build-essential
$ sudo apt install libcurl4-openssl-dev libmagic-dev
$ ./build_linux.sh
```

### Windows (MSYS2 on Windows 11)
in MSYS2 window
```
$ pacman -S git
$ pacman -S package-config
$ pacman -S mingw-w64-x86_64-gcc
$ pacman -S mingw-w64-x86_64-curl
```
in Mingw64 window
```
$ ./build_windows.sh
```

### macOS
```
$ brew install libmagic
$ ./build_macOS.sh
```


## Protocol

### Create a file sharing session.
```
POST /upload
     contentType:
       application/x-www-form-urlencoded
     res body:
       {sessionKey}
```

### Set metadata such as owner key.
```
PUT  /upload/{sessionKey}/set-metadata
     contentType:
       application/x-www-form-urlencoded
     req body:
       ownerKey={ownerKey}
```

### Create a file entry in sharing session.
```
PUT /upload/{sessionKey}/begin-file
     contentType:
       application/x-www-form-urlencoded
     req body:
       fileName={fileName}
       contentType={conentType}
     res body:
       {fileId}
```

### Send part of the binary data of the file.
```
PUT /upload/{sessionKey}/send-file
     contentType:
       multipart/form-data
     req body:
       file={fileData}
       fileId={fileId}
```
    
### Close a file entry in sharing session.
```
PUT /upload/{sessionKey}/end-file
     contentType:
       application/x-www-form-urlencoded
     req body:
       fileId={fileId}
```
    
### Close a file sharing session.
```
PUT /upload/{sessionKey}/end-session
     contentType:
       application/x-www-form-urlencoded
```

### Show the file sharing web page.
```
GET /share_{sessionKey}.html
```
