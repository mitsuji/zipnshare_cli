#ifndef __ARGS_HPP__
#define __ARGS_HPP__

#include <string>
#include <map>
#include "exception.hpp"

class args_exception : public zipnshare_exception {
public:
  args_exception (const std::string& what_arg)
    : zipnshare_exception (what_arg) {}
};


void parse_args(int argc, char ** argv,
		bool & help, bool & yes,
		std::map<std::string,std::string> & paths,
		std::string & owner_key, std::string & url);

#endif

