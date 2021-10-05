#include "file.hpp"
#include <filesystem>

bool file_exists(const std::string & path) {
  return std::filesystem::exists(path);
}

FILE * file_open_for_read(const std::string & path) {
  return fopen(path.c_str(),"r");
}

std::uintmax_t file_size(const std::string & path) {
  return std::filesystem::file_size(path);
}

bool is_absolute(const std::string & path) {
  std::filesystem::path fpath(path);
  return fpath.is_absolute();
}

const std::string file_name(const std::string & path) {
  std::filesystem::path fpath(path);
  return fpath.filename().string();
}
