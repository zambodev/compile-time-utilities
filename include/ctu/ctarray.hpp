#pragma once

#include <array>
#include <type_traits>

namespace ctu {

template <typename Type, Type... Is>
struct ctarray {
  static constexpr std::array<Type, sizeof...(Is)> arr = {Is...};
};

namespace Private {

// Get item at Index
// ------------------------------------------------------------------------------------------------
template <typename Array, unsigned long Index>
struct get;

template <typename Type, Type I0, Type... Is, unsigned long Index>
struct get<ctarray<Type, I0, Is...>, Index> {
  static constexpr Type value = get<ctarray<Type, Is...>, Index - 1>::value;
};

template <typename Type, Type I0, Type... Is>
struct get<ctarray<Type, I0, Is...>, 0> {
  static constexpr Type value = I0;
};

// Compare arrays
// ------------------------------------------------------------------------------------------------
template <typename Array1, typename Array2>
struct compare;

template <typename Type, Type I0, Type... Is, Type J0, Type... Js>
struct compare<ctarray<Type, I0, Is...>, ctarray<Type, J0, Js...>> {
  static constexpr bool value =
      (I0 == J0 ? compare<ctarray<Type, Is...>, ctarray<Type, Js...>>::value
                : false);
};

template <typename Type, Type... Is>
struct compare<ctarray<Type, Is...>, ctarray<Type>> {
  static constexpr bool value = false;
};

template <typename Type, Type... Js>
struct compare<ctarray<Type>, ctarray<Type, Js...>> {
  static constexpr bool value = false;
};

template <typename Type>
struct compare<ctarray<Type>, ctarray<Type>> {
  static constexpr bool value = true;
};

// Drop N items
// ------------------------------------------------------------------------------------------------
template <typename Array, unsigned long N>
struct drop;

template <typename Type, Type I0, Type... Is, unsigned long N>
struct drop<ctarray<Type, I0, Is...>, N> {
  using type = typename drop<ctarray<Type, Is...>, N - 1>::type;
};

template <typename Type, Type I0, Type... Is>
struct drop<ctarray<Type, I0, Is...>, 0> {
  using type = ctarray<Type, I0, Is...>;
};

template <typename Type>
struct drop<ctarray<Type>, 0> {
  using type = ctarray<Type>;
};

// Prepend item
// ------------------------------------------------------------------------------------------------
template <typename Type, Type Item, typename Array>
struct prepend;

template <typename Type, Type Item, Type... Is>
struct prepend<Type, Item, ctarray<Type, Is...>> {
  using type = ctarray<Type, Item, Is...>;
};

template <typename Type, Type Item>
struct prepend<Type, Item, ctarray<Type>> {
  using type = ctarray<Type, Item>;
};

// Take N items
// ------------------------------------------------------------------------------------------------
template <typename Array, unsigned long N>
struct take;

template <typename Type, Type I0, Type... Is, unsigned long N>
struct take<ctarray<Type, I0, Is...>, N> {
  using type =
      typename prepend<Type, I0,
                       typename take<ctarray<Type, Is...>, N - 1>::type>::type;
};

template <typename Type, Type I0, Type... Is>
struct take<ctarray<Type, I0, Is...>, 0> {
  using type = ctarray<Type>;
};

template <typename Type>
struct take<ctarray<Type>, 0> {
  using type = ctarray<Type>;
};

// Prepend N zeroes
// ------------------------------------------------------------------------------------------------
template <typename Type, typename Array, unsigned long N>
struct fit;

template <typename Type, Type... Is, unsigned long N>
struct fit<Type, ctarray<Type, Is...>, N> {
  using type =
      typename fit<Type,
                   typename prepend<Type, Type{}, ctarray<Type, Is...>>::type,
                   N - 1>::type;
};

template <typename Type, Type... Is>
struct fit<Type, ctarray<Type, Is...>, 0> {
  using type = ctarray<Type, Is...>;
};

// Search Item
// ------------------------------------------------------------------------------------------------
template <typename Type, typename Array, Type Item>
struct search;

template <typename Type, Type... Ls, Type Item>
struct search<Type, ctarray<Type, Ls...>, Item> {
  static constexpr unsigned long length = sizeof...(Ls);
  static constexpr unsigned long m0 =
      get<ctarray<Type, Ls...>, length / 2>::value;
  static constexpr unsigned long value =
      (Item == m0
           ? sizeof...(Ls)
           : (Item < m0
                  ? search<
                        Type,
                        typename take<ctarray<Type, Ls...>, length / 2>::type,
                        Item>::value
                  : search<
                        Type,
                        typename drop<ctarray<Type, Ls...>, length / 2>::type,
                        Item>::value));
};

template <typename Type, Type I0, Type Item>
struct search<Type, ctarray<Type, I0>, Item> {
  static constexpr unsigned long value = 0;
};

template <typename Type, Type Item>
struct search<Type, ctarray<Type>, Item> {
  static constexpr unsigned long value = 0;
};

// Merge two arrays
// ------------------------------------------------------------------------------------------------
template <typename Type, typename Array1, typename Array2>
struct merge;

template <typename Type, Type I0, Type... Is, Type J0, Type... Js>
struct merge<Type, ctarray<Type, I0, Is...>, ctarray<Type, J0, Js...>> {
  using type = std::conditional_t<
      (I0 < J0),
      typename prepend<Type, I0,
                       typename merge<Type, ctarray<Type, Is...>,
                                      ctarray<Type, J0, Js...>>::type>::type,
      typename prepend<Type, J0,
                       typename merge<Type, ctarray<Type, I0, Is...>,
                                      ctarray<Type, Js...>>::type>::type>;
};

template <typename Type, Type I0, Type J0, Type... Js>
struct merge<Type, ctarray<Type, I0>, ctarray<Type, J0, Js...>> {
  using type = std::conditional_t<
      (I0 < J0),
      typename prepend<Type, I0,
                       typename merge<Type, ctarray<Type>,
                                      ctarray<Type, J0, Js...>>::type>::type,
      typename prepend<Type, J0,
                       typename merge<Type, ctarray<Type, I0>,
                                      ctarray<Type, Js...>>::type>::type>;
};

template <typename Type, Type I0, Type... Is, Type J0>
struct merge<Type, ctarray<Type, I0, Is...>, ctarray<Type, J0>> {
  using type = std::conditional_t<
      (I0 < J0),
      typename prepend<Type, I0,
                       typename merge<Type, ctarray<Type, Is...>,
                                      ctarray<Type, J0>>::type>::type,
      typename prepend<Type, J0,
                       typename merge<Type, ctarray<Type, I0, Is...>,
                                      ctarray<Type>>::type>::type>;
};

template <typename Type, Type... Js>
struct merge<Type, ctarray<Type>, ctarray<Type, Js...>> {
  static constexpr unsigned long length = sizeof...(Js);
  using L = typename take<ctarray<Type, Js...>, length / 2>::type;
  using R = typename drop<ctarray<Type, Js...>, length / 2>::type;
  using type = typename merge<Type, L, R>::type;
};

template <typename Type, Type... Is>
struct merge<Type, ctarray<Type, Is...>, ctarray<Type>> {
  static constexpr unsigned long length = sizeof...(Is);
  using L = typename take<ctarray<Type, Is...>, length / 2>::type;
  using R = typename drop<ctarray<Type, Is...>, length / 2>::type;
  using type = typename merge<Type, L, R>::type;
};

template <typename Type, Type I0, Type J0>
struct merge<Type, ctarray<Type, I0>, ctarray<Type, J0>> {
  using type = std::conditional_t<(I0 < J0), ctarray<Type, I0, J0>,
                                  ctarray<Type, J0, I0>>;
};

// Sort Array
// ------------------------------------------------------------------------------------------------
template <typename Type, typename Array>
struct sort;

template <typename Type, Type... Is>
struct sort<Type, ctarray<Type, Is...>> {
  static constexpr unsigned long length = sizeof...(Is);
  using type = typename merge<
      Type,
      typename sort<
          Type, typename take<ctarray<Type, Is...>, length / 2>::type>::type,
      typename sort<Type, typename drop<ctarray<Type, Is...>,
                                        length / 2>::type>::type>::type;
};

template <typename Type, Type I0, Type J0>
struct sort<Type, ctarray<Type, I0, J0>> {
  using type = std::conditional_t<(I0 < J0), ctarray<Type, I0, J0>,
                                  ctarray<Type, J0, I0>>;
};

template <typename Type, Type I0>
struct sort<Type, ctarray<Type, I0>> {
  using type = ctarray<Type, I0>;
};

template <typename Type>
struct sort<Type, ctarray<Type>> {
  using type = ctarray<Type>;
};

// Normalize : Compute Item % Fact for each item in the array
// ------------------------------------------------------------------------------------------------
template <typename Type, typename Array, unsigned long Fact>
struct normalize;

template <typename Type, Type I0, Type... Is, unsigned long Fact>
struct normalize<Type, ctarray<Type, I0, Is...>, Fact> {
  using type = typename prepend<
      Type, I0 % Fact,
      typename normalize<Type, ctarray<Type, Is...>, Fact>::type>::type;
};

template <typename Type, Type I0, unsigned long Fact>
struct normalize<Type, ctarray<Type, I0>, Fact> {
  using type = ctarray<Type, I0 % Fact>;
};

// Doubles : Checks if a sorted array has a double
// ------------------------------------------------------------------------------------------------
template <typename Type, typename Array>
struct doubles;

template <typename Type, Type I0, Type I1, Type... Is>
struct doubles<Type, ctarray<Type, I0, I1, Is...>> {
  static constexpr bool value =
      (I0 == I1 ? true : doubles<Type, ctarray<Type, I1, Is...>>::value);
};

template <typename Type, Type I0, Type I1>
struct doubles<Type, ctarray<Type, I0, I1>> {
  static constexpr bool value = (I0 == I1 ? true : false);
};

// Concatenate two arrays
// ------------------------------------------------------------------------------------------------
template <typename Type, typename Array0, typename Array1>
struct concat;

template <typename Type, Type... Items0, Type... Items1>
struct concat<Type, ctarray<Type, Items0...>, ctarray<Type, Items1...>> {
  using type = ctarray<Type, Items0..., Items1...>;
};

}  // namespace Private
// ################################################################################################

template <typename Array, unsigned long Index>
constexpr inline auto ctarray_get_v = Private::get<Array, Index>::value;

template <typename Array1, typename Array2>
constexpr inline bool ctarray_cmp_v = Private::compare<Array1, Array2>::value;

template <typename Array, unsigned long N>
using ctarray_drop_t = Private::drop<Array, N>::type;

template <typename Array, unsigned long N>
using ctarray_take_t = Private::take<Array, N>::type;

template <typename Type, Type Item, typename Array>
using ctarray_prepend_t = Private::prepend<Type, Item, Array>::type;

template <typename Type, typename Array, Type Item>
constexpr inline auto ctarray_search_v =
    Private::search<Type, Array, Item>::value;

template <typename Type, typename Array>
using ctarray_sort_t = Private::sort<Type, Array>::type;

template <typename Type, typename Array, unsigned long N>
using ctarray_fit_t = Private::fit<Type, Array, N>::type;

template <typename Type, typename Array, unsigned long Fact>
using ctarray_norm_t = Private::normalize<Type, Array, Fact>::type;

template <typename Type, typename Array>
constexpr inline bool ctarray_doubles_v = Private::doubles<Type, Array>::value;

template <typename Type, typename Array0, typename Array1>
using ctarray_concat_t = Private::concat<Type, Array0, Array1>::type;

}  // namespace ctu
// ################################################################################################
