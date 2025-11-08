#pragma once

#include <array>
#include <type_traits>

namespace ctu {

template <typename Type, Type... Is>
struct array {
  static constexpr std::array<Type, sizeof...(Is)> arr = {Is...};
};

namespace Private {

// Get item
// ------------------------------------------------------------------------------------------------
template <typename Array, unsigned long Index>
struct get;

template <typename Type, Type I0, Type... Is, unsigned long Index>
struct get<array<Type, I0, Is...>, Index> {
  static constexpr Type value = get<array<Type, Is...>, Index - 1>::value;
};

template <typename Type, Type I0, Type... Is>
struct get<array<Type, I0, Is...>, 0> {
  static constexpr Type value = I0;
};

// Compare
// ------------------------------------------------------------------------------------------------
template <typename Array1, typename Array2>
struct compare;

template <typename Type, Type I0, Type... Is, Type J0, Type... Js>
struct compare<array<Type, I0, Is...>, array<Type, J0, Js...>> {
  static constexpr bool value =
      (I0 == J0 ? compare<array<Type, Is...>, array<Type, Js...>>::value
                : false);
};

template <typename Type, Type... Is>
struct compare<array<Type, Is...>, array<Type>> {
  static constexpr bool value = false;
};

template <typename Type, Type... Js>
struct compare<array<Type>, array<Type, Js...>> {
  static constexpr bool value = false;
};

template <typename Type>
struct compare<array<Type>, array<Type>> {
  static constexpr bool value = true;
};

// Drop item
// ------------------------------------------------------------------------------------------------
template <typename Array, unsigned long Count>
struct drop;

template <typename Type, Type I0, Type... Is, unsigned long Count>
struct drop<array<Type, I0, Is...>, Count> {
  using type = typename drop<array<Type, Is...>, Count - 1>::type;
};

template <typename Type, Type I0, Type... Is>
struct drop<array<Type, I0, Is...>, 0> {
  using type = array<Type, I0, Is...>;
};

template <typename Type>
struct drop<array<Type>, 0> {
  using type = array<Type>;
};

// Prepend item
// ------------------------------------------------------------------------------------------------
template <typename Type, Type I0, typename Array>
struct prepend;

template <typename Type, Type I0, Type... Is>
struct prepend<Type, I0, array<Type, Is...>> {
  using type = array<Type, I0, Is...>;
};

template <typename Type, Type I0>
struct prepend<Type, I0, array<Type>> {
  using type = array<Type, I0>;
};

// Take item
// ------------------------------------------------------------------------------------------------
template <typename Array, unsigned long Count>
struct take;

template <typename Type, Type I0, Type... Is, unsigned long Count>
struct take<array<Type, I0, Is...>, Count> {
  using type = typename prepend<
      Type, I0, typename take<array<Type, Is...>, Count - 1>::type>::type;
};

template <typename Type, Type I0, Type... Is>
struct take<array<Type, I0, Is...>, 0> {
  using type = array<Type>;
};

template <typename Type>
struct take<array<Type>, 0> {
  using type = array<Type>;
};

// Fit
// ------------------------------------------------------------------------------------------------
template <typename Type, typename Array, unsigned long Count>
struct fit;

template <typename Type, Type... Is, unsigned long Count>
struct fit<Type, array<Type, Is...>, Count> {
  using type =
      typename fit<Type,
                   typename prepend<Type, Type{}, array<Type, Is...>>::type,
                   Count - 1>::type;
};

template <typename Type, Type... Is>
struct fit<Type, array<Type, Is...>, 0> {
  using type = array<Type, Is...>;
};

// Search
// ------------------------------------------------------------------------------------------------
template <typename Type, typename Array, Type S>
struct search;

template <typename Type, Type... Ls, Type S>
struct search<Type, array<Type, Ls...>, S> {
  static constexpr unsigned long length = sizeof...(Ls);
  static constexpr unsigned long m0 =
      get<array<Type, Ls...>, length / 2>::value;
  static constexpr unsigned long value =
      (S == m0
           ? sizeof...(Ls)
           : (S < m0
                  ? search<Type,
                           typename take<array<Type, Ls...>, length / 2>::type,
                           S>::value
                  : search<Type,
                           typename drop<array<Type, Ls...>, length / 2>::type,
                           S>::value));
};

template <typename Type, Type I0, Type S>
struct search<Type, array<Type, I0>, S> {
  static constexpr unsigned long value = 0;
};

template <typename Type, Type S>
struct search<Type, array<Type>, S> {
  static constexpr unsigned long value = 0;
};

// Merge
// ------------------------------------------------------------------------------------------------
template <typename Type, typename Array1, typename Array2>
struct merge;

template <typename Type, Type I0, Type... Is, Type J0, Type... Js>
struct merge<Type, array<Type, I0, Is...>, array<Type, J0, Js...>> {
  using type = std::conditional_t<
      (I0 < J0),
      typename prepend<Type, I0,
                       typename merge<Type, array<Type, Is...>,
                                      array<Type, J0, Js...>>::type>::type,
      typename prepend<Type, J0,
                       typename merge<Type, array<Type, I0, Is...>,
                                      array<Type, Js...>>::type>::type>;
};

template <typename Type, Type I0, Type J0, Type... Js>
struct merge<Type, array<Type, I0>, array<Type, J0, Js...>> {
  using type = std::conditional_t<
      (I0 < J0),
      typename prepend<Type, I0,
                       typename merge<Type, array<Type>,
                                      array<Type, J0, Js...>>::type>::type,
      typename prepend<Type, J0,
                       typename merge<Type, array<Type, I0>,
                                      array<Type, Js...>>::type>::type>;
};

template <typename Type, Type I0, Type... Is, Type J0>
struct merge<Type, array<Type, I0, Is...>, array<Type, J0>> {
  using type = std::conditional_t<
      (I0 < J0),
      typename prepend<Type, I0,
                       typename merge<Type, array<Type, Is...>,
                                      array<Type, J0>>::type>::type,
      typename prepend<Type, J0,
                       typename merge<Type, array<Type, I0, Is...>,
                                      array<Type>>::type>::type>;
};

template <typename Type, Type... Js>
struct merge<Type, array<Type>, array<Type, Js...>> {
  static constexpr unsigned long length = sizeof...(Js);
  using L = typename take<array<Type, Js...>, length / 2>::type;
  using R = typename drop<array<Type, Js...>, length / 2>::type;
  using type = typename merge<Type, L, R>::type;
};

template <typename Type, Type... Is>
struct merge<Type, array<Type, Is...>, array<Type>> {
  static constexpr unsigned long length = sizeof...(Is);
  using L = typename take<array<Type, Is...>, length / 2>::type;
  using R = typename drop<array<Type, Is...>, length / 2>::type;
  using type = typename merge<Type, L, R>::type;
};

template <typename Type, Type I0, Type J0>
struct merge<Type, array<Type, I0>, array<Type, J0>> {
  using type =
      std::conditional_t<(I0 < J0), array<Type, I0, J0>, array<Type, J0, I0>>;
};

// Sort
// ------------------------------------------------------------------------------------------------
template <typename Type, typename Array>
struct sort;

template <typename Type, Type... Is>
struct sort<Type, array<Type, Is...>> {
  static constexpr unsigned long length = sizeof...(Is);
  using type = typename merge<
      Type,
      typename sort<Type,
                    typename take<array<Type, Is...>, length / 2>::type>::type,
      typename sort<Type, typename drop<array<Type, Is...>,
                                        length / 2>::type>::type>::type;
};

template <typename Type, Type I0, Type J0>
struct sort<Type, array<Type, I0, J0>> {
  using type =
      std::conditional_t<(I0 < J0), array<Type, I0, J0>, array<Type, J0, I0>>;
};

template <typename Type, Type I0>
struct sort<Type, array<Type, I0>> {
  using type = array<Type, I0>;
};

template <typename Type>
struct sort<Type, array<Type>> {
  using type = array<Type>;
};

// Normalize
// ------------------------------------------------------------------------------------------------
template <typename Type, typename Array, unsigned long Fact>
struct normalize;

template <typename Type, Type I0, Type... Is, unsigned long Fact>
struct normalize<Type, array<Type, I0, Is...>, Fact> {
  using type = typename prepend<
      Type, I0 % Fact,
      typename normalize<Type, array<Type, Is...>, Fact>::type>::type;
};

template <typename Type, Type I0, unsigned long Fact>
struct normalize<Type, array<Type, I0>, Fact> {
  using type = array<Type, I0 % Fact>;
};

// Doubles
// ------------------------------------------------------------------------------------------------
// Array needs to be sorted first
template <typename Type, typename Array>
struct doubles;

template <typename Type, Type I0, Type I1, Type... Is>
struct doubles<Type, array<Type, I0, I1, Is...>> {
  static constexpr bool value =
      (I0 == I1 ? true : doubles<Type, array<Type, I1, Is...>>::value);
};

template <typename Type, Type I0, Type I1>
struct doubles<Type, array<Type, I0, I1>> {
  static constexpr bool value = (I0 == I1 ? true : false);
};

// Concat
// ------------------------------------------------------------------------------------------------
template <typename Type, typename Array0, typename Array1>
struct concat;

template <typename Type, Type... Items0, Type... Items1>
struct concat<Type, array<Type, Items0...>, array<Type, Items1...>> {
  using type = array<Type, Items0..., Items1...>;
};

}  // namespace Private
// ################################################################################################

template <typename Array, unsigned long Index>
constexpr inline auto ctarray_get_v = Private::get<Array, Index>::value;

template <typename Array1, typename Array2>
constexpr inline bool ctarray_cmp_v = Private::compare<Array1, Array2>::value;

template <typename Array, unsigned long Count>
using ctarray_drop_t = Private::drop<Array, Count>::type;

template <typename Array, unsigned long Count>
using ctarray_take_t = Private::take<Array, Count>::type;

template <typename Type, Type Item, typename Array>
using ctarray_prepend_t = Private::prepend<Type, Item, Array>::type;

template <typename Type, typename Array, Type Item>
constexpr inline auto ctarray_search_v =
    Private::search<Type, Array, Item>::value;

template <typename Type, typename Array>
using ctarray_sort_t = Private::sort<Type, Array>::type;

template <typename Type, typename Array, unsigned long Count>
using ctarray_fit_t = Private::fit<Type, Array, Count>::type;

template <typename Type, typename Array, unsigned long Fact>
using ctarray_norm_t = Private::normalize<Type, Array, Fact>::type;

template <typename Type, typename Array>
constexpr inline bool ctarray_doubles_v = Private::doubles<Type, Array>::value;

template <typename Type, typename Array0, typename Array1>
using ctarray_concat_t = Private::concat<Type, Array0, Array1>::type;

}  // namespace ctu
// ################################################################################################
