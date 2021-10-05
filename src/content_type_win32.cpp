#include "content_type.hpp"
#include <windows.h>
#include <shlwapi.h>
#include <cstdio>


// $ gcc -std=c++17 src/content_type_win32.cpp src/exception.cpp -o dest/content_type -lstdc++ -lshlwapi
// $ ./dest/content_type file/file1.txt

#if 0
int main (int argc, char ** argv) {
  std::string content_type = get_content_type(argv[1]);
  printf("content_type: \'%s\'\n",content_type.c_str());
  return 0;
}
#endif


const std::string get_content_type(const char * path) {

  char * szExtension;
  szExtension = PathFindExtension(path);
  if (szExtension == NULL) {
    throw content_type_exception("failed to execute PathFindExtension");
  }

  char buff [256];
  DWORD dwType;;
  DWORD buff_len;
  buff_len = sizeof(buff);

  LSTATUS lStatus;
  lStatus = RegGetValue(HKEY_CLASSES_ROOT,szExtension,"Content Type",RRF_RT_REG_SZ,&dwType,buff,&buff_len);
  if (lStatus != ERROR_SUCCESS) {
    throw content_type_exception("failed to execute RegGetValue");
  }

  return std::string(buff,buff_len-1);
}
