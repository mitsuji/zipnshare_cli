#include "utf8.hpp"
#include "file.hpp"
#include "args.hpp"
#include <list>
#include <cstring>
#include <filesystem>
#include <sstream>
#include <cstdio>

// $ gcc -std=c++17 src/args.cpp src/exception.cpp src/utf8_none.cpp src/file_none.cpp -o dest/args -lstdc++
// $ ./dest/args
// $ ./dest/args --help
// $ ./dest/args file1 file2 file3
// $ ./dest/args file1 /file2 file3
// $ ./dest/args file1 /file2 file3 -ok foo
// $ ./dest/args file1 /file2 file3 -u http://localhost:8080
// $ ./dest/args file1 /file2 file3 -d /home/mitsuji
// $ ./dest/args -d /home/mitsuji
// $ ./dest/args -y -d /home/mitsuji

#if 0
int main (int argc, char ** argv) {

  bool help;
  bool yes;
  std::list<std::string> paths;
  std::string owner_key;
  std::string url;
  
  parse_args(argc, argv,
	     help, yes,
	     paths,
	     owner_key, url);

  printf("help:%d\n",help);
  printf("yes:%d\n",yes);
  printf("paths:\n");
  for (std::list<std::string>::const_iterator it = paths.begin();
       it != paths.end(); ++it) {
    printf("\t%s\n",it->c_str());
  }
  printf("owner_key: %s\n", owner_key.c_str());
  printf("url: %s\n", url.c_str());

  return 0;
}
#endif



#define OPTION_NONE 0
#define OPTION_DIRECTORY 1
#define OPTION_OWNER_KEY 2
#define OPTION_URL 3

void parse_args(int argc, char ** argv,
		bool & help, bool & yes,
		std::map<std::string,std::string> & paths,
		std::string & owner_key, std::string & url) {
  std::string directory;
  std::list<std::string> raw_paths;
  
  help = false;
  yes = false;

  // parse options
  int option = 0;
  for (int i = 1; i < argc; ++i) {
    switch(option) {
    case OPTION_NONE : {
      const std::string arg (argv[i]);
      if (arg.at(0) == '-') {
	if (false) {
	  //
	} else if (arg.compare("-h") == 0 || arg.compare("--help") == 0) {
	  help = true;
	} else if (arg.compare("-y") == 0 || arg.compare("--yes") == 0) {
	  yes = true;
	} else if (arg.compare("-d") == 0 || arg.compare("--dir") == 0) {
	  option = OPTION_DIRECTORY;
	} else if (arg.compare("-ok") == 0 || arg.compare("--owner_key") == 0) {
	  option = OPTION_OWNER_KEY;
	} else if (arg.compare("-u") == 0 || arg.compare("--url") == 0) {
	  option = OPTION_URL;
	} else {
	  std::ostringstream out_err;
	  out_err << "invalid option: " << arg;
	  throw args_exception(out_err.str());
	}
      } else {
	raw_paths.push_back(argv[i]);
      }
      break;
    }
    case OPTION_DIRECTORY : {
      directory = argv[i];
      option = OPTION_NONE;
      break;
    }
    case OPTION_OWNER_KEY : {
      owner_key = argv[i];
      option = OPTION_NONE;
      break;
    }
    case OPTION_URL : {
      url = argv[i];
      option = OPTION_NONE;
      break;
    }
    }
  }

  if (help) {
    return;
  }

  // when no file specified, append all files in directory
  if(raw_paths.size() == 0) {
    std::filesystem::directory_iterator it;
    if(!directory.empty()) {
      //  check existance
      if(!file_exists(directory)) {
	throw args_exception("directory not exists");
      }
      it = std::filesystem::directory_iterator (directory);
    } else {
      it = std::filesystem::directory_iterator ("."); // current
    }

    for (; it != std::filesystem::end(it); ++it) {
      if (it->is_regular_file() ) {
	// std Library returns filename in UTF-8 Linux/macOS/MinGW
	raw_paths.push_back(ansi_from_utf8(it->path().filename().string()));
      }
    }
  }

  // when directory specified, append path
  if(!directory.empty()) {
    for (std::list<std::string>::const_iterator it = raw_paths.begin();
	 it != raw_paths.end(); ++it) {
      if(!is_absolute(*it)) {
	std::ostringstream out_path;
	out_path << directory;
	out_path << '/' << *it;
	const std::string & path = out_path.str();
	paths.insert_or_assign(file_name(path),path);
      } else {
	paths.insert_or_assign(file_name(*it),*it);
      }
    }
  } else {
    for (std::list<std::string>::const_iterator it = raw_paths.begin();
	 it != raw_paths.end(); ++it) {
      paths.insert_or_assign(file_name(*it),*it);
    }
  }

}
