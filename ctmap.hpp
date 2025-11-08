#pragma once

#include <ctarray.hpp>
#include <ctpair.hpp>
#include <ctstring.hpp>
#include <stdexcept>
#include <utility>

namespace ctu {

template <typename Type>
using mappair = ctpair<ctstring<64>, Type>;

namespace Private {

// Map to std::array
// ------------------------------------------------------------------------------------------------
template <typename Type, typename Array>
struct map_to_stdarray;

template <typename Type, mappair<Type>... Ps>
struct map_to_stdarray<Type, ctarray<mappair<Type>, Ps...>> {
  static constexpr std::array<Type, sizeof...(Ps)> value = {{{Ps.second}...}};
};

// Pad
// ------------------------------------------------------------------------------------------------
template <typename Type, typename Array, unsigned int Last = 0>
struct pad;

template <typename Type, mappair<Type> I0, mappair<Type>... Is,
          unsigned int Last>
struct pad<Type, ctarray<mappair<Type>, I0, Is...>, Last> {
  using fit_item = ctarray_fit_t<mappair<Type>, ctarray<mappair<Type>, I0>,
                                 (Last == 0 ? I0.first.crc32 - Last
                                            : I0.first.crc32 - Last - 1)>;
  using type = ctarray_concat_t<
      mappair<Type>, fit_item,
      typename pad<Type, ctarray<mappair<Type>, Is...>, I0.first.crc32>::type>;
};

template <typename Type, mappair<Type> I0, unsigned int Last>
struct pad<Type, ctarray<mappair<Type>, I0>, Last> {
  using type = ctarray_fit_t<mappair<Type>, ctarray<mappair<Type>, I0>,
                             I0.first.crc32 - Last - 1>;
};

template <typename Type, unsigned long Fact, mappair<Type>... Pairs>
struct map {
  using norm_arr =
      ctarray_norm_t<mappair<Type>, ctarray<mappair<Type>, Pairs...>, Fact>;
  using sorted_arr = ctarray_sort_t<mappair<Type>, norm_arr>;
  static_assert(ctarray_doubles_v<mappair<Type>, sorted_arr> == false);
  using padded = typename pad<Type, sorted_arr>::type;

  using stdarray = std::array<Type, padded::arr.size()>;
  static constexpr stdarray arr = map_to_stdarray<Type, padded>::value;
};
}  // namespace Private
// ################################################################################################

// ctmap
// ------------------------------------------------------------------------------------------------
template <typename Type, unsigned long Fact, mappair<Type>... Pairs>
class ctmap {
 public:
  constexpr ctmap() = default;
  constexpr ~ctmap() = default;

  Type operator[](const ctstring<64>& str) const {
    unsigned int idx = str.get_crc32() % Fact;

    if (idx >= cmap::arr.size()) throw std::runtime_error("Key not found");

    return cmap::arr[idx];
  }

  void print(void) const {
    for (unsigned long i = 0; i < cmap::arr.size(); ++i)
      std::cout << "[" << i << "] = " << cmap::arr[i] << "\n";
  }

 private:
  using cmap = Private::map<Type, Fact, Pairs...>;
};

}  // namespace ctu
// ################################################################################################
