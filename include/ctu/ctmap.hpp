#pragma once

#include <ctu/ctarray.hpp>
#include <ctu/ctpair.hpp>
#include <ctu/ctstring.hpp>
#include <expected>
#include <type_traits>
#include <utility>
#include <string>
#include <array>

namespace ctu {

namespace Private {

// Map to std::array
// ------------------------------------------------------------------------------------------------
template <typename FirstType, typename SecondType, typename Array>
struct map_to_stdarray;

template <typename FirstType, typename SecondType,
          ctpair<FirstType, SecondType>... Ps>
struct map_to_stdarray<FirstType, SecondType,
                       ctarray<ctpair<FirstType, SecondType>, Ps...>> {
  static constexpr std::array<std::pair<FirstType, SecondType>, sizeof...(Ps)> value = {
      {{std::pair{Ps.first, Ps.second}}...}};
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

// Map : Yup, looks like a map to me
// ------------------------------------------------------------------------------------------------
template <typename FirstType, typename SecondType, unsigned long Fact,
          bool Found, typename Array>
struct map;

template <typename FirstType, typename SecondType, unsigned long Fact,
          ctpair<FirstType, SecondType>... Ps>
struct map<FirstType, SecondType, Fact, false,
           ctarray<ctpair<FirstType, SecondType>, Ps...>> {
  using norm_arr =
      ctarray_norm_t<ctpair<FirstType, SecondType>,
                     ctarray<ctpair<FirstType, SecondType>, Ps...>, Fact>;
  using sorted_arr = ctarray_sort_t<ctpair<FirstType, SecondType>, norm_arr>;

  static constexpr bool doubles_found =
      ctarray_doubles_v<ctpair<FirstType, SecondType>, sorted_arr>;

  using func =
      std::conditional_t<doubles_found,
                         map<FirstType, SecondType, Fact + 1, false,
                             ctarray<ctpair<FirstType, SecondType>, Ps...>>,
                         map<FirstType, SecondType, Fact, true, sorted_arr>>;
  using type = typename func::type;
  static constexpr unsigned long value = func::value;
};

template <typename FirstType, typename SecondType, unsigned long Fact,
          typename Array>
struct map<FirstType, SecondType, Fact, true, Array> {
  using type = typename pad<FirstType, SecondType, Array>::type;
  static constexpr unsigned long value = Fact;
};

}  // namespace Private
// ################################################################################################

// ctmap
// Humanly usable map wrapper
// ------------------------------------------------------------------------------------------------
template <typename FirstType, typename SecondType,
          ctpair<FirstType, SecondType>... Pairs>
class ctmap {
 public:
  constexpr ctmap() = default;
  constexpr ~ctmap() = default;

  auto operator[](const ctstring<64>& key) const -> std::expected<SecondType, std::string> {
    unsigned int idx = (key % cmap_v).crc32;

    if (idx >= arr.size())
      return std::unexpected{"Key not found!"};

    if (key.crc32 != arr[idx].first.crc32_bak)
      return std::unexpected{"Key not found!"};


    return arr[idx].second;
  }

  template <std::integral T>
  auto operator[](const T& key) const -> std::expected<SecondType, std::string> {
    unsigned int idx = key % cmap_v;

    if (idx >= arr.size())
      return std::unexpected{"Key not found!"};

    return arr[idx].second;
  }

 private:
  using cmap = Private::map<FirstType, SecondType, sizeof...(Pairs), false,
                            ctarray<ctpair<FirstType, SecondType>, Pairs...>>;
  using cmap_t = typename cmap::type;
  static constexpr unsigned long cmap_v = cmap::value;

  using stdarray = std::array<std::pair<FirstType, SecondType>, cmap_t::arr.size()>;
  static constexpr stdarray arr =
      ctu::Private::map_to_stdarray<FirstType, SecondType, cmap_t>::value;
};

}  // namespace ctu
// ################################################################################################
