#include "utf8.hpp"

const std::string to_utf8(const std::wstring & in) {
  throw utf8_exception ("to_utf8 is implemented on windows only");
}

const std::wstring from_utf8(const std::string & in) {
  throw utf8_exception ("from_utf8 is implemented on windows only");
}


const std::wstring to_wide(const std::string & in) {
  throw utf8_exception ("to_wide is implemented on windows only");
}

const std::string from_wide(const std::wstring & in) {
  throw utf8_exception ("from_wide is implemented on windows only");
}


const std::string ansi_to_utf8(const std::string & in) {
  return in;
}

const std::string ansi_from_utf8(const std::string & in) {
  return in;
}

