#include <TextBuffer.h>

namespace qsl {

const char* const TextBuffer::LITERAL_TRUE = "true";
const char* const TextBuffer::LITERAL_FALSE = "false";
const size_t TextBuffer::LITERAL_LEN_TRUE = strlen(TextBuffer::LITERAL_TRUE);
const size_t TextBuffer::LITERAL_LEN_FALSE = strlen(TextBuffer::LITERAL_FALSE);

TextBuffer::TextBuffer(size_t capacity) noexcept : size_(0), capacity_(capacity),
    truncated_(false), str_(nullptr), memPool_(nullptr) {
  if (capacity_ > 0) {
    str_ = static_cast<char*>(::malloc(capacity_ + 1));
    if (str_ != nullptr) {
      str_[0] = '\0';
    } else {
      capacity_ = 0;
    }
  }
}

TextBuffer::TextBuffer(MemPool& pool, size_t capacity) noexcept :
                       size_(0), capacity_(capacity), truncated_(false),
                       str_(nullptr), memPool_(&pool) {
  if (capacity_ > 0) {
    str_ = static_cast<char*>(memPool_->malloc(capacity_ + 1));
    if (str_ != nullptr) {
      str_[0] = '\0';
    } else {
      capacity_ = 0;
    }
  }
}

TextBuffer::~TextBuffer() noexcept {
  free();
}

TextBuffer& TextBuffer::transferTextBuffer(TextBuffer& other) noexcept {
  if (&other != this) {
    free();
    size_ = other.size_;
    capacity_ = other.capacity_;
    truncated_ = other.truncated_;
    str_ = other.str_;
    memPool_ = other.memPool_;
    other.size_ = 0;
    other.capacity_ = 0;
    other.truncated_ = false;
    other.str_ = nullptr;
    other.memPool_ = nullptr;
  }
  return *this;
}

void TextBuffer::swap(TextBuffer& other) noexcept {
  std::swap(size_, other.size_);
  std::swap(capacity_, other.capacity_);
  std::swap(truncated_, other.truncated_);
  std::swap(str_, other.str_);
  std::swap(memPool_, other.memPool_);
}

void TextBuffer::clear() noexcept {
  if (str_ != nullptr && capacity_ > 0) {
    str_[0] = '\0';
  }
  size_ = 0;
}

bool TextBuffer::reserve(size_t capacity) noexcept {
  if (capacity > capacity_) {
    if (capacity < capacity_ * 2) {
      capacity *= 2;
    }
    char* p = static_cast<char*>((nullptr == memPool_ ?
        ::malloc(capacity + 1) : memPool_->malloc(capacity + 1)));
    if (nullptr == p) {
      return false;
    }
    if (str_ != nullptr) {
      memcpy(p, str_, size_ + 1);
      free();
    }
    str_ = p;
    capacity_ = capacity;
  }
  return true;
}

TextBuffer& TextBuffer::push(const char* str) noexcept {
  if (str != nullptr) {
    push(str, strlen(str));
  } else {
    truncated_ = false;
  }
  return *this;
}

TextBuffer& TextBuffer::push(const char* str, size_t len) noexcept {
  if ((str != nullptr) && (len != 0)) {
    if (len + size_ <= capacity_) {
      memcpy(str_ + size_, str, len);
      size_ += len;
      str_[size_] = '\0';
      truncated_ = false;
    } else {
      if (reserve(len + size_)) {
        memcpy(str_ + size_, str, len);
        size_ += len;
        str_[size_] = '\0';
        truncated_ = false;
      } else if (size_ < capacity_) {
        if (size_ < capacity_) {
          memcpy(str_ + size_, str, capacity_ - size_);
          size_ = capacity_;
          str_[size_] = '\0';
        }
        truncated_ = true;
      }
    }
  }
  return *this;
}

TextBuffer& TextBuffer::vpushf(const char* fmt, va_list args) noexcept {
  if (nullptr == fmt) {
    return *this;
  }

  va_list argTmp;
  size_t len = 0;

  if (str_ != nullptr) {
    va_copy(argTmp, args);
    len = vsnprintf(str_ + size_, capacity_ - size_ + 1, fmt, argTmp);
    va_end(argTmp);
    if (len > 0 && size_ + len <= capacity_) {
      size_ += len;
      truncated_ = false;
      return *this;
    }
  } else {
    va_copy(argTmp, args);
    len = vsnprintf(str_ + size_, capacity_ - size_ + 1, fmt, argTmp);
    va_end(argTmp);
  }

  if (len > 0) {
    if (reserve(size_ + len)) {
      va_copy(argTmp, args);
      len = vsnprintf(str_ + size_, capacity_ - size_ + 1, fmt, argTmp);
      va_end(argTmp);
      size_ += len;
      truncated_ = false;
    } else {
      size_ = capacity_;
      truncated_ = true;
    }
  }
  return *this;
};


TextBuffer& TextBuffer::pushf(const char* fmt, ...) noexcept {
  va_list args;
  va_start(args, fmt);
  vpushf(fmt, args);
  va_end(args);
  return *this;
}

}
