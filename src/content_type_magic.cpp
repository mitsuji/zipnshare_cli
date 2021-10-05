#include "content_type.hpp"
#include <magic.h>
#include <cstdio>


// $ gcc -std=c++17 src/content_type_magic.cpp src/exception.cpp -o dest/content_type -lstdc++ -lmagic
// $ ./dest/content_type file/file1.txt

#if 0
int main (int argc, char ** argv) {
  std::string content_type = get_content_type(argv[1]);
  printf("content_type: \'%s\'\n",content_type.c_str());
  return 0;
}
#endif


class magic_box {
public:
  magic_box () {
    magic_cookie = magic_open (MAGIC_MIME);
    if( magic_cookie == NULL ) {
      throw content_type_exception("failed to execute magic_open");
    }
  }
  virtual ~magic_box () {
    magic_close (magic_cookie);
#if 0
    printf("~magic_box\n");
#endif
  }
  magic_t get () const {
    return magic_cookie;
  }
private:
  magic_t magic_cookie;
};


// [prerequisite] file at the path exists

const std::string get_content_type(const char * path) {
  
  magic_box _magic_box;
  int status_load = magic_load (_magic_box.get(),NULL); // use default magic file
  if(status_load) {
    throw content_type_exception("failed to execute magic_load");
  }

  const char * magic_result_cstr = magic_file (_magic_box.get(), path);
  if(magic_result_cstr == NULL) {
    throw content_type_exception("failed to execute magic_file");
  }

  std::string magic_result (magic_result_cstr);
  size_t pos_semicolon = magic_result.find(';');
  return magic_result.substr(0,pos_semicolon);

}
