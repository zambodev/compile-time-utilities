#pragma once

#include <cstring>
#include <ctu/ctarray.hpp>

namespace ctu {

// ctstring
// ------------------------------------------------------------------------------------------------
template <unsigned long Size>
struct ctstring {
  unsigned int crc32 = {0};
  char c_str[Size] = {0};

  constexpr ctstring() : crc32{0}, c_str{0} {};
  constexpr ctstring(const char* str) {
    unsigned long idx = 0;
    const char* p = str;

    while (*p != '\0' && idx < Size) {
      c_str[idx] = *p;
      ++idx;
      ++p;
    }

    this->crc32 = this->get_crc32();
  }

  constexpr ctstring(const char* str, unsigned int crc) {
    unsigned long idx = 0;
    const char* p = str;

    while (*p != '\0' && idx < Size) {
      c_str[idx] = *p;
      ++idx;
      ++p;
    }

    crc32 = crc;
  }

  constexpr ctstring(const ctstring<Size>& str) {
    memcpy(c_str, str.c_str, Size);
    crc32 = str.crc32;
  }

  constexpr ctstring(ctstring<Size>&& str) {
    memcpy(c_str, str.c_str, Size);
    crc32 = str.crc32;
  }

  constexpr ~ctstring() {}

  constexpr ctstring& operator=(const ctstring<Size>& str) {
    for (unsigned long i = 0; i < Size; ++i) c_str[i] = str.c_str[i];
    crc32 = str.crc32;
    return *this;
  }

  template <unsigned long StrSize = Size>
  constexpr auto operator<=>(const ctstring<StrSize>& str) const {
    return (crc32 <=> str.crc32);
  }

  template <unsigned long StrSize = Size>
  constexpr auto operator==(const ctstring<StrSize>& str) const {
    return (crc32 == str.crc32);
  }

  constexpr auto operator%(const unsigned int val) const {
    return ctstring<Size>(static_cast<const char*>(c_str), crc32 % val);
  }

  constexpr unsigned int operator-(const unsigned int val) const {
    return crc32 - val;
  }

  constexpr unsigned int get_crc32(void) const {
    unsigned int crc = 0xFFFFFFFF;

    for (unsigned long i = 0; i < Size; ++i) {
      crc ^= static_cast<unsigned char>(c_str[i]);
      for (int i = 0; i < 8; ++i)
        crc = (crc >> 1) ^ (0xEDB88320 & (-(crc & 1)));
    }

    return ~crc;
  }
};

}  // namespace ctu
// ################################################################################################
