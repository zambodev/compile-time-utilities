#pragma once

#include <ctarray.hpp>

namespace ctu {

// ctstring
// ------------------------------------------------------------------------------------------------
struct ctstring {
  static constexpr unsigned long SIZE = 64;

  bool is_empty = true;
  char c_str[SIZE] = {0};

  constexpr ctstring() : c_str{0}, is_empty{true} {};
  constexpr ctstring(const char* str) {
    unsigned long idx = 0;
    const char* p = str;

    while (*p != '\0' && idx < SIZE) {
      c_str[idx] = *p;
      ++idx;
      ++p;
    }

    is_empty = false;
  }

  constexpr ~ctstring() {}

  constexpr auto operator<=>(const ctstring& str) const {
    auto crc = this->get_crc32();
    auto strcrc = str.get_crc32();

    return (crc <=> strcrc);
  }

  constexpr auto operator<(const ctstring& str) const {
    auto crc = this->get_crc32();
    auto strcrc = str.get_crc32();

    return (crc < strcrc);
  }

  constexpr unsigned int get_crc32(void) const {
    unsigned int crc = 0xFFFFFFFF;

    for (unsigned long i = 0; i < SIZE; ++i) {
      crc ^= static_cast<unsigned char>(c_str[i]);
      for (int i = 0; i < 8; ++i)
        crc = (crc >> 1) ^ (0xEDB88320 & (-(crc & 1)));
    }

    return ~crc;
  }

  static constexpr unsigned int static_crc32(const char* str) {
    ctstring cstr(str);
    return cstr.get_crc32();
  }
};

}  // namespace ctu
// ################################################################################################
