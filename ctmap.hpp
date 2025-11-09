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
template <typename FirstType, typename SecondType, typename Array>
struct map_to_stdarray;

template <typename FirstType, typename SecondType,
          ctpair<FirstType, SecondType>... Ps>
struct map_to_stdarray<FirstType, SecondType,
                       ctarray<ctpair<FirstType, SecondType>, Ps...>> {
  static constexpr std::array<SecondType, sizeof...(Ps)> value = {
      {{Ps.second}...}};
};

// Pad
// ------------------------------------------------------------------------------------------------
template <typename FirstType, typename SecondType, typename Array,
          unsigned int Last = 0>
struct pad;

template <typename FirstType, typename SecondType,
          ctpair<FirstType, SecondType> I0, ctpair<FirstType, SecondType>... Is,
          unsigned int Last>
struct pad<FirstType, SecondType,
           ctarray<ctpair<FirstType, SecondType>, I0, Is...>, Last> {
  using fit_item =
      ctarray_fit_t<ctpair<FirstType, SecondType>,
                    ctarray<ctpair<FirstType, SecondType>, I0>,
                    (Last == 0 ? I0.first - Last : I0.first - Last - 1)>;
  using type = ctarray_concat_t<
      ctpair<FirstType, SecondType>, fit_item,
      typename pad<FirstType, SecondType,
                   ctarray<ctpair<FirstType, SecondType>, Is...>,
                   I0.first - 0>::type>;
};

template <typename FirstType, typename SecondType,
          ctpair<FirstType, SecondType> I0, unsigned int Last>
struct pad<FirstType, SecondType, ctarray<ctpair<FirstType, SecondType>, I0>,
           Last> {
  using type = ctarray_fit_t<ctpair<FirstType, SecondType>,
                             ctarray<ctpair<FirstType, SecondType>, I0>,
                             I0.first - Last - 1>;
};

template <typename FirstType, typename SecondType, unsigned long Fact,
          ctpair<FirstType, SecondType>... Pairs>
struct map {
  using norm_arr =
      ctarray_norm_t<ctpair<FirstType, SecondType>,
                     ctarray<ctpair<FirstType, SecondType>, Pairs...>, Fact>;
  using sorted_arr = ctarray_sort_t<ctpair<FirstType, SecondType>, norm_arr>;
  static_assert(ctarray_doubles_v<ctpair<FirstType, SecondType>, sorted_arr> ==
                false);
  using padded = typename pad<FirstType, SecondType, sorted_arr>::type;

  using stdarray = std::array<SecondType, padded::arr.size()>;
  static constexpr stdarray arr =
      map_to_stdarray<FirstType, SecondType, padded>::value;
};
}  // namespace Private
// ################################################################################################

// ctmap
// ------------------------------------------------------------------------------------------------
template <typename FirstType, typename SecondType, unsigned long Fact,
          ctpair<FirstType, SecondType>... Pairs>
class ctmap {
 public:
  constexpr ctmap() = default;
  constexpr ~ctmap() = default;

  SecondType operator[](const ctstring<64>& key) const {
    unsigned int idx = (key % Fact).crc32;

    if (idx >= cmap::arr.size()) throw std::runtime_error("Key not found");

    return cmap::arr[idx];
  }

  template <std::integral T>
  SecondType operator[](const T& key) const {
    unsigned int idx = key % Fact;

    if (idx >= cmap::arr.size()) throw std::runtime_error("Key not found");

    return cmap::arr[idx];
  }

  void print(void) const {
    for (unsigned long i = 0; i < cmap::arr.size(); ++i)
      std::cout << "[" << i << "] = " << cmap::arr[i] << "\n";
  }

 private:
  using cmap = Private::map<FirstType, SecondType, Fact, Pairs...>;
};

}  // namespace ctu
// ################################################################################################
