#ifndef UTIL__HPP_
#define UTIL__HPP_

#include <algorithm>

// trim string
static inline std::string &trim(std::string &s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
  s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
  return s;
}

static inline std::string str_tolower(std::string s) {
  std::transform (s.begin(), s.end(), s.begin(), (int(*)(int))std::tolower);

  return s;
}

#endif
