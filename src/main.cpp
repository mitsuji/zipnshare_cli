#include "exception.hpp"
#include "utf8.hpp"
#include "args.hpp"
#include "file.hpp"
#include "content_type.hpp"
#include "upload.hpp"
#include <curl/curl.h>
#include <filesystem>
#include <vector>
#include <cstdio>
#include <sstream>
#include <iostream>

#define APP_BIN_NAME "zipnshare-cli"

class curl_global_box {
public:
  curl_global_box () {
    curl_global_init(CURL_GLOBAL_ALL);
  }
  virtual ~curl_global_box () {
    curl_global_cleanup();
#if 0
    printf("~curl_global_box\n");
#endif
  }
};

class file_box {
public:
  file_box (const std::string & path) {
    fd = file_open_for_read(path.c_str());
    if (fd == NULL) {
      throw zipnshare_exception("failed to execute file_open_for_read");
    }
  }
  virtual ~file_box () {
    fclose(fd);
#if 0
    printf("~file_box\n");
#endif
  }
  FILE * get () const {
    return fd;
  }
private:
  FILE * fd;
};

int main (int argc, char ** argv) {
#ifdef __MINGW32__
  curl_global_sslset(CURLSSLBACKEND_SCHANNEL, NULL, NULL);
#endif
  curl_global_box _curl_global_box;
  
  try {
    bool help;
    bool yes;
    std::map<std::string,std::string> paths;
    std::string owner_key;
    std::string url = "http://localhost:8080/zipnshare";

    parse_args(argc, argv,
	       help, yes,
	       paths,
	       owner_key, url);
    
    if(help) {
      printf("%s - the file share tool\n",APP_BIN_NAME);
      printf("\n");
      printf("Usage: %s [options] [paths]\n",APP_BIN_NAME);
      printf("\n");

      printf("Available options:\n");
      printf("  -h,--help\t\t");      printf("Show this help text\n");
      printf("  -y,--yes\t\t");       printf("Automatic yes to prompts\n");
      printf("  -d,--dir\t\t");       printf("Specify Directory\n");
      printf("  -ok,--owner-key\t");  printf("Specify Owner Key\n");
      printf("  -u,--url\t\t");       printf("Specify zipnshare server\n");
      printf("\n");

      printf("Example:\n");
      printf("\t# upload all files in current directory\n");
      printf("\t$ %s\n",APP_BIN_NAME);
      printf("\n");
      printf("\t# upload all files in current directory without confirmation\n");
      printf("\t$ %s -y\n",APP_BIN_NAME);
      printf("\n");
      printf("\t# upload file foo.txt specified by relative path\n");
      printf("\t$ %s foo.txt\n",APP_BIN_NAME);
      printf("\n");
      printf("\t# upload file foo.txt specified by absolute path\n");
      printf("\t$ %s /home/someone/foo.txt\n",APP_BIN_NAME);
      printf("\n");
      printf("\t# upload some files\n");
      printf("\t$ %s foo1.txt foo2.txt /home/someone/foo.txt\n",APP_BIN_NAME);
      printf("\n");
      printf("\t# upload all files in directory /home/someone/somedir \n");
      printf("\t$ %s --dir /home/someone/somedir\n",APP_BIN_NAME);
      printf("\n");
      printf("\t# upload file foo.txt in directory /home/someone/somedir \n");
      printf("\t$ %s --dir /home/someone/somedir foo.txt\n",APP_BIN_NAME);
      printf("\n");
      return 0;
    }

    // [MEMO] check file cont
    if(paths.size() > 20) {
      std::ostringstream out_err;
      out_err << "too many files specified. paths.size(): " << paths.size();
      throw zipnshare_exception(out_err.str());
    }

    std::uintmax_t total_size = 0;
    for (std::map<std::string,std::string>::const_iterator it = paths.begin();
	 it != paths.end(); ++it) {
      std::string path (it->second);

      // [MEMO] check file existence
      if(!file_exists(path)){
	std::ostringstream out_err;
	out_err << "file not exists. path: " << path;
	throw zipnshare_exception(out_err.str());
      }
    
      std::uintmax_t fsize = file_size(path);
      // [MEMO] check file size zero
      if(fsize == 0){
	std::ostringstream out_err;
	out_err << "file size is zero. path: " << path;
	throw zipnshare_exception(out_err.str());
      }
      // [MEMO] check file size 2GB
      if(fsize > 2ULL * 1024 * 1024 * 1024){
	std::ostringstream out_err;
	out_err << "file size is too large. path: " << path << " fsize: " << fsize;
	throw zipnshare_exception(out_err.str());
      }
    
#if 0
      printf("%s: %ld\n",path.c_str(),fsize);
#endif
      total_size += fsize;
    }
#if 0
    printf("total_size: %ld\n",total_size);
#endif


    if(!yes) {
      printf("Files to share:\n");
      for (std::map<std::string,std::string>::const_iterator it = paths.begin();
	   it != paths.end(); ++it) {
	std::string path (it->second);
	printf("\t%s\n",path.c_str());
      }
      printf("Do you want to share these files with someone? [y/n]\n");
      char conf;
      std::cin >> conf;
      if (conf != 'y') {
	return 0;
      }
    }

    // [TODO] show progress file process in byte
    // [TODO] show progress total process in byte
    // [TODO] show estimate time of total process
    std::string session_key = upload_init (url);
    upload_set_metadata (url, session_key, owner_key);

    for (std::map<std::string,std::string>::const_iterator it = paths.begin();
	 it != paths.end(); ++it) {
      std::string file_name (it->first);
      std::string path (it->second);

      printf("%s",path.c_str());

      std::string content_type;
      try {
	content_type = get_content_type(path.c_str());
      } catch (content_type_exception & ex) {
	content_type = "";
      }

      std::string file_id = upload_begin_file (url, session_key,
					       file_name, content_type);
      printf(" ");
      {
	file_box _file_box(path);

	size_t buff_len = 20ULL * 1024 * 1024;
	std::vector<char> buff (buff_len);
	size_t read_len;
	while(!feof(_file_box.get())) {
	  read_len = fread(buff.data(),sizeof(char),buff_len,_file_box.get());
#if 0
	  printf("\tread_len: %zd\n",read_len);
#endif
	  upload_send_file (url, session_key,
			    file_id, buff.data(), read_len);
	  printf(".");
	  fflush(stdout);
	}
      }
      printf("OK\n");

      upload_end_file (url, session_key, file_id);

    }

    upload_end_session (url, session_key);

    printf("\n");
    printf("File sharing is ready in the URL below!!\n");
    printf("\t%s/share_%s.html\n",url.c_str(),session_key.c_str());
    return 0;

  } catch (args_exception & ex) {
    fprintf(stderr,"args_exception: %s\n",ex.what());
    return 3;
  } catch (content_type_exception & ex) {
    fprintf(stderr,"content_type_exception: %s\n",ex.what());
    return 4;
  } catch (upload_exception & ex) {
    fprintf(stderr,"upload_exception: %s\n",ex.what());
    return 5;
  } catch (utf8_exception & ex) {
    fprintf(stderr,"utf8_exception: %s\n",ex.what());
    return 6;
  } catch (zipnshare_exception & ex) {
    fprintf(stderr,"zipnshare_exception: %s\n",ex.what());
    return 2;
  } catch (std::exception & ex) {
    fprintf(stderr,"std::exception: %s\n",ex.what());
    return 1;
  }
}
