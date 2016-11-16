#include <qsl/pool/InflatePool.h>

namespace qsl {
  int InflatePool::create(size_t maxBlockSize, size_t bmin,
                          size_t bmax, float increaseRate) {
    return -1;
  }

  void destroy() {

  }

  void* malloc(size_t size) {
    return nullptr;
  }

  void free(void* p, size_t size) {

  }

  void clear() {

  }


}
