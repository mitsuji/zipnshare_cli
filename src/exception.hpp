#ifndef __EXCEPTION_HPP__
#define __EXCEPTION_HPP__

#include <string>
#include <stdexcept>

class zipnshare_exception : public std::exception
{
public:
  explicit zipnshare_exception(const std::string& what_arg);
  virtual const char* what() const noexcept;
private:
  std::string what_arg;
};

#endif
