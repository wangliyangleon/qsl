#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <new>

#include <gsl/gsl>

#define SAFE_FREE_MEMORY(ptr) do {    \
  if (ptr != nullptr) {               \
    ::free(ptr);                      \
    ptr = nullptr;                    \
  }                                   \
} while (0);

#define SAFE_DELETE_ARRAY(array) do {   \
  if (array != nullptr) {               \
    delete [] array;                    \
    array = nullptr;                    \
  }                                     \
} while(0);

#define SAFE_DELETE_OBJECT(object) do { \
  if (object != nullptr) {              \
    delete object;                      \
    object = nullptr;                   \
  }                                     \
} while(0);
