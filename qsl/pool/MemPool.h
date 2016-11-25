#pragma once

#include "../CommonInclude.h"

namespace qsl {

class MemPool {
public:
  virtual ~MemPool() {}
  virtual void* malloc(size_t size) noexcept = 0;
  virtual void free(void* p, size_t size) noexcept = 0;
};

/// system pool, directly use malloc and free
class SysPool : public MemPool {
public:
  virtual void* malloc(size_t size) noexcept {
    return ::malloc(size);
  }

  virtual void free(void* p, size_t) noexcept {
    ::free(p);
  }
};

}
