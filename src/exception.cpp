#include "exception.hpp"

zipnshare_exception::zipnshare_exception(const std::string& what_arg)
  : what_arg (what_arg) {}
const char* zipnshare_exception::what() const noexcept {
  return what_arg.c_str();
}

