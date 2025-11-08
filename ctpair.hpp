#pragma once

#include <ctarray.hpp>
#include <ctstring.hpp>

namespace ctu {

// ctpair
// ------------------------------------------------------------------------------------------------
template <typename FirstType, typename SecondType>
struct ctpair {
  FirstType first{};
  SecondType second{};

  constexpr ctpair() : first{}, second{} {}

  constexpr ctpair(const FirstType& key, const SecondType& value) {
    this->first = key;
    this->second = value;
  }

  constexpr ctpair(const ctpair& pair) {
    this->first = pair.first;
    this->second = pair.second;
  }

  constexpr auto operator<=>(const ctpair& pair) const {
    return (this->first <=> pair.first);
  }

  constexpr bool operator==(const ctpair& pair) const {
    return (this->first == pair.first);
  }

  constexpr auto operator%(const unsigned long val) const {
    return ctpair<FirstType, SecondType>(first % val, second);
  }
};

}  // namespace ctu
// ################################################################################################
