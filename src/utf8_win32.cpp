#include "utf8.hpp"
#include <windows.h>
#include <vector>

const std::string to_utf8(const std::wstring & in) {
  if(in.length() == 0) {
    return "";
  }
  
  int buff_len = WideCharToMultiByte (CP_UTF8, 0, in.c_str(), in.length(), NULL, 0, NULL, NULL);
  std::vector<char> buff (buff_len);
  if(!WideCharToMultiByte (CP_UTF8, 0, in.c_str(), in.length(), buff.data(), buff_len, NULL, NULL)){
    throw utf8_exception("failed to execute WideCharToMultiByte");
  }

  return std::string(buff.data(),buff_len);
}

const std::wstring from_utf8(const std::string & in) {
  if(in.length() == 0) {
    return L"";
  }

  int buff_len = MultiByteToWideChar (CP_UTF8, 0, in.c_str(), in.length(), NULL, 0);
  std::vector<wchar_t> buff (buff_len);
  if(!MultiByteToWideChar (CP_UTF8, 0, in.c_str(), in.length(), buff.data(), buff_len)) {
    throw utf8_exception("failed to execute MultiByteToWideChar");
  }

  return std::wstring(buff.data(),buff_len);
}


const std::wstring to_wide(const std::string & in) {
  if(in.length() == 0) {
    return L"";
  }

  int buff_len = MultiByteToWideChar (CP_ACP, 0, in.c_str(), in.length(), NULL, 0);
  std::vector<wchar_t> buff (buff_len);
  if(!MultiByteToWideChar (CP_ACP, 0, in.c_str(), in.length(), buff.data(), buff_len)) {
    throw utf8_exception("failed to execute MultiByteToWideChar");
  }

  return std::wstring(buff.data(),buff_len);
}

const std::string from_wide(const std::wstring & in) {
  if(in.length() == 0) {
    return "";
  }

  int buff_len = WideCharToMultiByte (CP_ACP, 0, in.c_str(), in.length(), NULL, 0, NULL, NULL);
  std::vector<char> buff (buff_len);
  if(!WideCharToMultiByte (CP_ACP, 0, in.c_str(), in.length(), buff.data(), buff_len, NULL, NULL)){
    throw utf8_exception("failed to execute WideCharToMultiByte");
  }

  return std::string(buff.data(),buff_len);
}


const std::string ansi_to_utf8(const std::string & in) {
  return to_utf8(to_wide(in));
}

const std::string ansi_from_utf8(const std::string & in) {
  return from_wide(from_utf8(in));
}



