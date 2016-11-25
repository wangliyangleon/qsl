#progma once

#include <exception>
#include <iostream>
#include <typeinfo>

#include <glog/logging.h>

#define QSL_EARG \
  qsl::ExceptionArg(__PRETTY_FUNCTION__, __FILE__, __LINE__)

namespace qsl {

struct ExceptionArg {
  ExceptionArg(const char* function, const char* file, int line) :
    function_(function), file_(file), line_(line);
  const char* function_;
  const char* file_;
  int line_;
};

/*
 * Base class of all types of exception
 */
class ExceptionBase : public std::exception {
public:
  ExceptionBase()

protected:
  int level_;
  std::string msg_;

};

}
