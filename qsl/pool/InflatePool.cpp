#include <pool/InflatePool.h>

namespace qsl {
int InflatePool::create(size_t maxBlockSize, size_t blockMinSize,
                        size_t blockMaxSize, float increaseRate) noexcept {
  destroy();
  buffer_ = ::malloc(maxBlockSize);
  if (nullptr == buffer_) {
    return -1;
  }
  bufferSize_ = maxBlockSize;

  int ret = pool_.create(buffer_, bufferSize_,
                         blockMinSize, blockMaxSize, increaseRate);
  if (ret != 0) {
    destroy();
    return -1;
  }
  maxBlockSize_ = std::max(bufferSize_, pool_.maxAllocSize());
  totalSize_ = bufferSize_;
  return 0;
}

void InflatePool::destroy() noexcept {
  pool_.destroy();
  for (auto buf : bufferSet_) {
    SAFE_FREE_MEMORY(buf);
  }
  bufferSet_.clear();
  SAFE_FREE_MEMORY(buffer_);

  bufferSize_ = 0;
  maxBlockSize_ = 0;
  totalSize_ = 0;
  consumedSize_ = 0;
}

void* InflatePool::malloc(size_t size) noexcept {
  void* memRet = nullptr;
  consumedSize_ += size;

  /// alloc new buffer
  if (size > pool_.maxAllocSize()) {
    memRet = ::malloc(size);
    if (memRet != nullptr) {
      bufferSet_.insert(memRet);
      totalSize_ += size;
      return memRet;
    }
  }

  /// get momory from pool
  memRet = pool_.malloc(size);
  if (memRet != nullptr) {
    return memRet;
  }

  /// add buffer to pool
  memRet = ::malloc(maxBlockSize_);
  if (memRet != nullptr) {
    bufferSet_.insert(memRet);
    pool_.addBuffer(memRet, maxBlockSize_);
    totalSize_ += maxBlockSize_;
    return pool_.malloc(size);
  }

  return nullptr;
}

void InflatePool::free(void* p, size_t size) noexcept {
  consumedSize_ -= size;
  if (size > pool_.maxAllocSize()) {
    bufferSet_.erase(p);
    SAFE_FREE_MEMORY(p);
    totalSize_ -= size;
  } else {
    pool_.free(p, size);
  }
}

void InflatePool::clear() noexcept {
  pool_.free(buffer_, bufferSize_);
  for (auto buf : bufferSet_) {
    SAFE_FREE_MEMORY(buf);
  }
  bufferSet_.clear();
  totalSize_ = bufferSize_;
  consumedSize_ = 0;
}

}
