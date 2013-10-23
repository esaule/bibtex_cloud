#ifndef EXCEPT_HPP
#define EXCEPT_HPP

#include "util/stacktrace.hpp"
#include "log.hpp"

class BaseException {
  std::vector<std::string> stack;

public:
  BaseException() {
    stack = get_backtrace();
  }   
  void print_backtrace() {
    for (auto l : stack)
      Log::log<<l<<Log::endl;

  }
};

namespace tact {
  namespace engine {

    class InvalidFormat  : public BaseException {
    };
    
  }
}

#endif
