#include <pool/AutoMemPool.h>

namespace qsl {
#define ROUND_SIZE(size) \
    ((size + sizeof(int) - 1) / sizeof(int) * sizeof(int))

int AutoMemPool::create(void* buffer, size_t size, size_t minSize,
                         size_t maxSize, float increaseRate) noexcept {
  if (nullptr == buffer || 0 == size || increaseRate < 1.01f) {
    return -1;
  }

  size_t minSizeR = std::max(ROUND_SIZE(minSize), sizeof(void*));
  size_t maxSizeR = ROUND_SIZE(maxSize);
  if (minSizeR > maxSizeR) {
    return -1;
  }

  poolSize_ = 1;
  for (size_t s = minSizeR; s < maxSizeR; ++poolSize_) {
    s = ROUND_SIZE(static_cast<size_t>(static_cast<float>(s) * increaseRate));
  }

  allocator_.create(buffer, size);
  minSize_ = minSizeR;
  maxSize_ = maxSizeR;
  increaseRate_ = increaseRate;
  reset();
  return 0;
}

void AutoMemPool::destroy() noexcept {
  SAFE_DELETE_ARRAY(pool_);
  allocator_.destroy();
  poolSize_ = 0;
  maxSize_ = 0;
  minSize_ = 0;
  increaseRate_ = 0.0f;
}

void* AutoMemPool::malloc(size_t size) noexcept {
  int index = getIndex(size);
  if (index >= 0) {
    void* p = pool_[index].malloc();
    if (p != nullptr) {
      /// return the free memory
      return p;
    }
    /// alloc new memory from the buffer
    return allocator_.malloc(pool_[index].getSize());
  }
  return nullptr;
}

void AutoMemPool::free(void* p, size_t size) noexcept {
  if (p != nullptr) {
    int index = getIndex(size);
    if (index >= 0) {
      pool_[index].free(p);
    }
  }
}

int AutoMemPool::reset(void* buffer, size_t size) noexcept {
  allocator_.create(buffer, size);
  SAFE_DELETE_ARRAY(pool_);
  pool_ = new (std::nothrow) MemManager[poolSize_];
  if (nullptr == pool_) {
    return -1;
  }

  maxSize_ = 0;
  size_t s = minSize_;
  for (size_t i = 0; i < poolSize_; ++i) {
    pool_[i].create(s);
    maxSize_ = s;
    s = ROUND_SIZE(static_cast<size_t>(static_cast<float>(s) * increaseRate_));
  }
  return 0;
}

/// DANGER
void* AutoMemPool::addBuffer(void* buffer, size_t size) noexcept {
  void* oldBuffer = static_cast<void*>(allocator_.getBuffer());
  for (size_t i = 0; i < poolSize_; ++i) {
    void* ret = nullptr;
    while ((ret = allocator_.malloc(pool_[i].getSize())) != nullptr) {
      pool_[i].free(ret);
    }
  }
  allocator_.create(buffer, size);
  return oldBuffer;
}

int AutoMemPool::getIndex(size_t size) noexcept {
  /// binary search, find the lower bound of size in the pool vector
  if (size > pool_[poolSize_ - 1].getSize()) {
    return -1;
  }
  int len = poolSize_ - 1;
  size_t first = 0;
  size_t mid = 0;
  size_t half = 0;
  while (len > 0) {
    half = len >> 1;
    mid = first + half;
    if (pool_[mid].getSize() < size) {
      first = mid + 1;
      len = len - half - 1;
    } else {
      len = half;
    }
  }
  return static_cast<int>(first);
}

}
