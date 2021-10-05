#include "file.hpp"
#include "utf8.hpp"
#include <filesystem>
#include <windows.h>


bool file_exists(const std::string & path) {
  std::wstring path_wide = to_wide(path);
  return std::filesystem::exists(path_wide);
}

FILE * file_open_for_read(const std::string & path) {
  std::wstring path_wide = to_wide(path);
  return _wfopen(path_wide.c_str(),L"rb");
}

std::uintmax_t file_size(const std::string & path) {
  std::wstring path_wide = to_wide(path);
  return std::filesystem::file_size(path_wide);
}

bool is_absolute(const std::string & path) {
  std::wstring path_wide = to_wide(path);
  std::filesystem::path fpath(path_wide);
  return fpath.is_absolute();
}

const std::string file_name(const std::string & path) {
  std::wstring path_wide = to_wide(path);
  std::filesystem::path fpath(path_wide);
  std::wstring filename_wide = fpath.filename().wstring();
  return from_wide(filename_wide);
}
