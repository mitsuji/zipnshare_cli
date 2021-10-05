#ifndef __FILE_HPP__
#define __FILE_HPP__

#include <string>
#include <cstdio>


bool file_exists(const std::string & path);

FILE * file_open_for_read(const std::string & path);

std::uintmax_t file_size(const std::string & path);

bool is_absolute(const std::string & path);

const std::string file_name(const std::string & path);

#endif
