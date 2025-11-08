#pragma once

#include <ctarray.hpp>
#include <ctpair.hpp>
#include <ctstring.hpp>
#include <stdexcept>
#include <utility>

namespace ctu {
namespace Private {
// Map to std::array
// ------------------------------------------------------------------------------------------------
template <typename Type, typename Array>
struct map_to_stdarray;

template <typename Type, ctpair<Type>... Ps>
struct map_to_stdarray<Type, array<ctpair<Type>, Ps...>> {
  static constexpr std::array<Type, sizeof...(Ps)> value = {{{Ps.second}...}};
};

// Pad
// ------------------------------------------------------------------------------------------------
template <typename Type, typename Array, unsigned int Last = 0>
struct pad;

template <typename Type, ctpair<Type> I0, ctpair<Type>... Is, unsigned int Last>
struct pad<Type, array<ctpair<Type>, I0, Is...>, Last> {
  using fit_item =
      ctarray_fit_t<ctpair<Type>, array<ctpair<Type>, I0>,
                    (Last == 0 ? I0.crc - Last : I0.crc - Last - 1)>;
  using type = ctarray_concat_t<
      ctpair<Type>, fit_item,
      typename pad<Type, array<ctpair<Type>, Is...>, I0.crc>::type>;
};

template <typename Type, ctpair<Type> I0, unsigned int Last>
struct pad<Type, array<ctpair<Type>, I0>, Last> {
  using type =
      ctarray_fit_t<ctpair<Type>, array<ctpair<Type>, I0>, I0.crc - Last - 1>;
};

template <typename Type, unsigned long Fact, ctpair<Type>... Pairs>
struct map {
  using norm_arr =
      ctarray_norm_t<ctpair<Type>, array<ctpair<Type>, Pairs...>, Fact>;
  using sorted_arr = ctarray_sort_t<ctpair<Type>, norm_arr>;
  static_assert(ctarray_doubles_v<ctpair<Type>, norm_arr> == false);
  using padded = typename pad<Type, sorted_arr>::type;

  using stdarray = std::array<Type, padded::arr.size()>;
  static constexpr stdarray arr = map_to_stdarray<Type, padded>::value;
};
}  // namespace Private
// ################################################################################################

// ctmap
// ------------------------------------------------------------------------------------------------
template <typename Type, unsigned long Fact, ctpair<Type>... Pairs>
class ctmap {
 public:
  constexpr ctmap() = default;
  constexpr ~ctmap() = default;

  Type operator[](const ctstring& str) const {
    unsigned int idx = str.get_crc32() % Fact;

    if (idx >= cmap::arr.size()) throw std::runtime_error("Key not found");

    return cmap::arr[idx];
  }

 private:
  using cmap = Private::map<Type, Fact, Pairs...>;
};

}  // namespace ctu
// ################################################################################################
