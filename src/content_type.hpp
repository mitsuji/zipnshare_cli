#ifndef __CONTENT_TYPE_HPP__
#define __CONTENT_TYPE_HPP__

#include <string>
#include "exception.hpp"

class content_type_exception : public zipnshare_exception {
public:
  content_type_exception (const std::string& what_arg)
    : zipnshare_exception (what_arg) {}
};

const std::string get_content_type(const char * path);

#endif
