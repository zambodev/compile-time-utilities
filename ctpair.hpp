#pragma once

#include <ctarray.hpp>
#include <ctstring.hpp>

namespace ctu {

// ctpair
// ------------------------------------------------------------------------------------------------
template <typename Type>
struct ctpair {
  unsigned int crc{};
  ctstring first{};
  Type second{};

  constexpr ctpair() : crc{}, first{}, second{} {}

  constexpr ctpair(const ctstring& key, const Type& value, unsigned long crc) {
    this->first = key;
    this->second = value;
    this->crc = crc;
  }

  constexpr ctpair(const ctstring& key, const Type& value) {
    this->crc = key.get_crc32();
    this->first = key;
    this->second = value;
  }

  constexpr ctpair(const ctpair& pair) {
    this->crc = pair.crc;
    this->first = pair.first;
    this->second = pair.second;
  }

  constexpr auto operator<=>(const ctpair& pair) const {
    return (this->crc <=> pair.crc);
  }

  constexpr bool operator==(const ctpair& pair) const {
    return (this->crc == pair.crc);
  }

  constexpr auto operator<(const ctpair& pair) const {
    return (this->crc < pair.crc);
  }

  constexpr auto operator%(const unsigned long val) const {
    return ctpair<Type>(first, second, crc % val);
  }
};

}  // namespace ctu
// ################################################################################################
