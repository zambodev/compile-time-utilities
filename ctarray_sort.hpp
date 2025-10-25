#include <cstdint>
#include "ctarr_utils.hpp"
#include "ct_arr_cmp.hpp"



// Drop item --------------------------------------------------------------------------------------
template <typename Array, unsigned long Index>
struct drop;

template <Type I0, Type... Is, unsigned long Index>
struct drop<ctarray<I0, Is...>, Index>
{
    using type = typename drop<ctarray<Is...>, Index - 1>::type;
};

template <Type I0, Type... Is>
struct drop<ctarray<I0, Is...>, 0>
{
    using type = ctarray<I0, Is...>;
};

template <>
struct drop<ctarray<>, 0>
{
    using type = ctarray<>;
};

// Prepend item -----------------------------------------------------------------------------------
template <Type I0, typename Array>
struct prepend;

template <Type I0, Type... Is>
struct prepend<I0, ctarray<Is...>>
{
    using type = ctarray<I0, Is...>;
};

template <Type I0>
struct prepend<I0, ctarray<>>
{
    using type = ctarray<I0>;
};

// Take item --------------------------------------------------------------------------------------
template <typename Array, unsigned long Index>
struct take;

template <Type I0, Type... Is, unsigned long Index>
struct take<ctarray<I0, Is...>, Index>
{
    using type = typename prepend<I0, typename take<ctarray<Is...>, Index - 1>::type>::type;
};

template <Type I0, Type... Is>
struct take<ctarray<I0, Is...>, 0>
{
    using type = ctarray<>;
};

template <>
struct take<ctarray<>, 0>
{
    using type = ctarray<>;
};

// Compare array ----------------------------------------------------------------------------------


// Search item ------------------------------------------------------------------------------------
template <typename Array, Type S>
struct search;

template <Type... Ls, Type S>
struct search<ctarray<Ls...>, S>
{
    static constexpr std::size_t length = sizeof...(Ls);
    static constexpr std::size_t m0 = get<ctarray<Ls...>, length / 2>::value;
    static constexpr std::size_t value = (S == m0
                                            ? sizeof...(Ls)
                                            : (S < m0
                                               ? search<typename take<ctarray<Ls...>, length / 2>::type, S>::value
                                               : search<typename drop<ctarray<Ls...>, length / 2>::type, S>::value));
};

template <Type I0, Type S>
struct search<ctarray<I0>, S>
{
    static constexpr std::size_t value = 0;
};

template <Type S>
struct search<ctarray<>, S>
{
    static constexpr std::size_t value = 0;
};

// Tests ------------------------------------------------------------------------------------------
// static_assert(search<ctarray<1,2,3>, 2>::index == 1);
