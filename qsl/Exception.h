#progma once

#include <exception>
#include <iostream>
#include <typeinfo>

#include <glog/logging.h>

#include "TextBuffer.h"

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
  typedef std::exception base_exception_type;

  ExceptionBase() : noexcept
      base_exception_type(),
      level_(0 /* TODO : use the level def in glog*/),
      stack_(DFT_STACK_CAPACITY),
      func_(DFT_FUNC_CAPACITY),
      msg_(DFT_MSG_CAPACITY),
      all_(0),
      arg_(DFT_FUNC_NAME, DFT_STACK_CAPACITY, DFT_LINE_NO) {}

  ExceptionBase(const ExceptionBase& other) : noexcept
      base_exception_type(other),
      level_(other.level_),
      stack_(other.stack_.capacity()),
      func_(other.func_.capacity()),
      msg_(other.msg_.capacity()),
      all_(0),
      arg_(other.arg_) {
    stack_.transferTextBuffer(other.stack_);
    func_.transferTextBuffer(other.func_);
    msg_.transferTextBuffer(other.msg_);
  }

  virtual ~ExceptionBase() noexcept {}

public:
  static const size_t DFT_STACK_CAPACITY = 64;
  static const size_t DFT_FUNC_CAPACITY = 32;
  static const size_t DFT_MSG_CAPACITY = 256;
  static const char* const DFT_EXCEP_FILE_NAME;
  static const char* const DFT_EXCEP_FUNC_NAME;
  static const uint32_t DFT_LINE_NO = 0u;
protected:
  int level_;
  mutable TextBuffer stack_;
  mutable TextBuffer func_;
  mutable TextBuffer msg_;
  mutable TextBuffer all_;
  ExceptionArg arg_;
};

}
