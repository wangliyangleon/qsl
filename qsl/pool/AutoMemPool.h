#pragma once

#include <stdio.h>

#include <gsl/gsl_byte>

#include "MemPool.h"

using namespace gsl;

namespace qsl {

union MemNode {
  MemNode* next;
  byte data[0];
};


class MemManager {
public:
  MemManager() noexcept {
    create(0);
  }

  ~MemManager() noexcept {
    create(0);
  }

  void create(size_t size) noexcept {
    size_ = size;
    freeList_ = nullptr;
  }

  void* malloc() noexcept {
    if (freeList_ != nullptr) {
      MemNode* node = freeList_;
      freeList_ = freeList_->next;
      return static_cast<void*>(node->data);
    }
    return nullptr;
  }

  void free(void* p) noexcept {
    static_cast<MemNode*>(p)->next = freeList_;
    freeList_ = static_cast<MemNode*>(p);
  }

private:
  MemNode* freeList_;
  size_t size_;
};


/// Manage a big block of memory and does not free
class NoFreePool : public MemPool {
  void create(void* buffer, size_t size) noexcept {
    buffer_ = static_cast<byte*>(buffer);
    bufferCapacity_ = size;
    bufferLeft_ = bufferCapacity_;
    bufferFree_ = 0;
  }

  void* malloc(size_t size) noexcept {
    if (size <= bufferLeft_) {
      bufferLeft_ -= size;
      return bufferLeft_ + bufferLeft_;
    }
    return nullptr;
  }

  void free(void*, size_t size) noexcept {
    bufferFree_ += size;
    if (bufferFree_ >= bufferCapacity_) {
      clear();
    }
  }

  void clear() noexcept {
    bufferLeft_ = bufferCapacity_;
    bufferFree_ = 0;
  }

  void destroy() noexcept {
    buffer_ = nullptr;
    bufferCapacity_ = 0;
    bufferLeft_ = 0;
    bufferFree_ = 0;
  }

  byte* getBuffer() noexcept {
    return buffer_;
  }
private:
  byte* buffer_;
  size_t* bufferCapacity_;
  size_t bufferLeft_;
  size_t bufferFree_;
};


class AutoMemPool : public MemPool {
public:
  AutoMemPool() : pool_(nullptr), poolSize_(0),
    minSize_(0), maxSize_(0), increaseRate_(0.0f) noexcept {}

  ~AutoMemPool() noexcept {
    destroy();
  }

  int create(void* buffer, size_t size,
             size_t minSize = sizeof(void*),
             size_t maxSize = (1 << 20) /* 1m */,
             float increaseRate = 2.0f) noexcept;

  void destroy() noexcept;

  void* malloc(size_t size) noexcept;

  void free(void* p, size_t size) noexcept;

  size_t maxAllocSize() noexcept {
    return maxSize_;
  }

  int reset(void* buffer, size_t size) noexcept;

  int reset() noexcept {
    return reset(buffer_, bufferCapacity_);
  }

  /// DANGER
  void* addBuffer(void* buffer, size_t size) noexcept;

private:
  MemManager* pool_;      /// memory pool
  NoFreePool allocator_;  /// memory allocator
  size_t poolSize_;
  size_t minSize_;
  size_t maxSize_;
  float increaseRate_;

  size_t getIndex(size_t size) noexcept;

};

}
