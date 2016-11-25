#pragma once

#include <set>

#include "AutoMemPool.h"

namespace qsl {

class InflatePool : public MemPool {
public:
  InflatePool() noexcept : maxBlockSize_(0), buffer_(nullptr), bufferSize_(0),
      totalSize_(0), consumedSize_(0) {}

  ~InflatePool() noexcept {
    destroy();
  }

  size_t getTotalSize() noexcept {
    return totalSize_;
  }

  size_t getConsumedSize() noexcept {
    return consumedSize_;
  }

  size_t getFreeSize() noexcept {
    return totalSize_ - consumedSize_;
  }

  int create(size_t maxBlockSize = (1 << 20) /* 1m */,
             size_t blockMinSize = sizeof(void*),
             size_t blockMaxSize = (1 << 16),
             float increaseRate = 2.0f) noexcept;

  void destroy() noexcept;

  void* malloc(size_t size) noexcept;

  void free(void* p, size_t size) noexcept;

  void clear() noexcept;


private:
  size_t maxBlockSize_;
  void* buffer_;
  size_t bufferSize_;
  AutoMemPool pool_;

  std::set<void*> bufferSet_;
  size_t totalSize_;
  size_t consumedSize_;
};

}
