#pragma once

#include "AutoMemPool.h"

namespace qsl {

class InflatePool : public MemPool {
public:
  InflatePool() : maxBlockSize_(0), buffer_(nullptr), bufferSize_(0),
      totalSize_(0), consumedSize_(0) {}

  ~InflatePool() {
    destroy();
  }

  size_t getTotalSize() {
    return totalSize_;
  }

  size_t getConsumedSize() {
    return consumedSize_;
  }

  size_t getFreeSize() {
    return totalSize_ - consumedSize_;
  }

  int create(size_t maxBlockSize = (1 << 20) /* 1m */,
             size_t bmin = sizeof(void*),
             size_t bmax = (1 << 16),
             float increaseRate = 2.0f);

  void destroy();

  void* malloc(size_t size);

  void free(void* p, size_t size);

  void clear();


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
