#pragma once

#include <string>

#include "CommonInclude.h"
#include "pool/MemPool.h"

namespace qsl {

class TextBuffer {
public:
  explicit TextBuffer(size_t capacity = DEFAULT_CAPACITY) noexcept;
  explicit TextBuffer(MemPool& pool,
                      size_t capacity = DEFAULT_CAPACITY) noexcept;
  ~TextBuffer() noexcept;

  TextBuffer& transferTextBuffer(TextBuffer& other) noexcept;
  void swap(TextBuffer& other) noexcept;

  size_t size() const noexcept {
    return size_;
  }

  size_t capacity() const noexcept {
    return capacity_;
  }

  bool empty() const noexcept {
    return size_ == 0;
  }

  bool full() const noexcept {
    return size_ == capacity_;
  }

  bool truncated() const noexcept {
    return truncated_;
  }

  const char* c_str() const noexcept {
    return nullptr == str_ ? "" : str_;
  }

  void clear() noexcept;
  bool reserve(size_t capacity) noexcept;

  TextBuffer& push(char c) noexcept {
    if (c != '\0') {
      if (full()) {
        reserve(size_ + 1);
      }
      if (full()) {
        truncated_ = true;
      } else {
        str_[size_] = c;
        str_[++size_] = '\0';
        truncated_ = false;
      }
    }
    return *this;
  }
  TextBuffer& push(const char* str) noexcept;
  TextBuffer& push(const char* str, size_t len) noexcept;
  TextBuffer& vpushf(const char* fmt, va_list args) noexcept;
  TextBuffer& pushf(const char* fmt, ...) noexcept
      __attribute__((format (printf, 2, 3)));

  TextBuffer& operator << (const TextBuffer& other) noexcept {
    return push(other.c_str(), other.size());
  }
  TextBuffer& operator << (const std::string& str) noexcept {
    return push(str.c_str(), str.length());
  }
  TextBuffer& operator << (const char* str) noexcept {
    return push(str);
  }
  TextBuffer& operator << (const char c) noexcept {
    return push(c);
  }
  TextBuffer& operator << (const bool flag) noexcept {
      if (flag) {
          return push(LITERAL_TRUE, LITERAL_LEN_TRUE);
      } else {
          return push(LITERAL_FALSE, LITERAL_LEN_FALSE);
      }
  }
  TextBuffer& operator << (signed char sc) noexcept {
    return pushf("%hhd", sc);
  }
  TextBuffer& operator << (unsigned char uc) noexcept {
    return pushf("%hhu", uc);
  }
  TextBuffer& operator << (wchar_t wc) noexcept {
    return pushf("%lc", wc);
  }
  TextBuffer& operator << (const wchar_t *ws) noexcept {
      if (ws != nullptr) {
          pushf("%ls", ws);
      } else {
          truncated_ = false;
      }
      return *this;
  }
  TextBuffer& operator << (short s) noexcept {
    return pushf("%hd", s);
  }
  TextBuffer& operator << (unsigned short s) noexcept {
    return pushf("%hu", s);
  }
  TextBuffer& operator << (int i) noexcept {
    return pushf("%d", i);
  }
  TextBuffer& operator << (unsigned int u) noexcept {
    return pushf("%u", u);
  }
  TextBuffer& operator << (long l) noexcept {
    return pushf("%ld", l);
  }
  TextBuffer& operator << (unsigned long ul) noexcept {
    return pushf("%lu", ul);
  }
  TextBuffer& operator << (long long ll) noexcept {
    return pushf("%lld", ll);
  }
  TextBuffer& operator << (unsigned long long ull) noexcept {
    return pushf("%llu", ull);
  }
  TextBuffer& operator << (double d) noexcept {
    return pushf("%lg", d);
  }
  TextBuffer& operator << (long double d) noexcept {
    return pushf("%Lg", d);
  }
  TextBuffer& operator << (void *p) noexcept {
    return pushf("%p", p);
  }
  TextBuffer& operator << (float f) noexcept {
    return pushf("%f", f);
  }

public:
  static const size_t DEFAULT_CAPACITY = 64;
  static const char* const LITERAL_TRUE;
  static const char* const LITERAL_FALSE;

private:
  TextBuffer(const TextBuffer& other) noexcept;
  TextBuffer& operator = (const TextBuffer& other) noexcept;

  void free() noexcept {
    if (nullptr == memPool_) {
      SAFE_FREE_MEMORY(str_);
    } else {
      memPool_->free(str_, capacity_);
    }
  }

  size_t size_;
  size_t capacity_;
  bool truncated_;
  char* str_;
  MemPool* memPool_;

  static const size_t LITERAL_LEN_TRUE;
  static const size_t LITERAL_LEN_FALSE;
};

}
