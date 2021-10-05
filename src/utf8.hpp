#ifndef __UTF8_HPP__
#define __UTF8_HPP__

#include <string>
#include "exception.hpp"

class utf8_exception : public zipnshare_exception {
public:
  utf8_exception (const std::string& what_arg)
    : zipnshare_exception (what_arg) {}
};

// wchar_t <-> UTF-8
const std::string to_utf8(const std::wstring & in);

const std::wstring from_utf8(const std::string & in);


// system encode <-> wchar_t
const std::wstring to_wide(const std::string & in);

const std::string from_wide(const std::wstring & in);


// system encode <-> UTF-8
const std::string ansi_to_utf8(const std::string & in);

const std::string ansi_from_utf8(const std::string & in);


#endif
