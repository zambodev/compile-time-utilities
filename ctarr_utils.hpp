using Type = int;

template <Type... Is>
struct ctarray;

namespace Private
{
    // Get item -----------------------------------------------------------------------------------
    template <typename Array, unsigned long Index>
    struct get;

    template <Type I0, Type... Is, unsigned long Index>
    struct get<ctarray<I0, Is...>, Index>
    {
        static constexpr Type value = get<ctarray<Is...>, Index - 1>::value;
    };

    template <Type I0, Type... Is>
    struct get<ctarray<I0, Is...>, 0>
    {
        static constexpr Type value = I0;
    };

    // Compare ------------------------------------------------------------------------------------
    template <typename Array1, typename Array2>
    struct compare;

    template <Type I0, Type... Is, Type J0, Type... Js>
    struct compare<ctarray<I0, Is...>, ctarray<J0, Js...>>
    {
        static constexpr bool value = (I0 == J0
                                       ? compare<ctarray<Is...>, ctarray<Js...>>::value
                                       : false);
    };

    template <Type... Is>
    struct compare<ctarray<Is...>, ctarray<>>
    {
        static constexpr bool value = false;
    };

    template <Type... Js>
    struct compare<ctarray<>, ctarray<Js...>>
    {
        static constexpr bool value = false;
    };

    template <>
    struct compare<ctarray<>, ctarray<>>
    {
        static constexpr bool value = true;
    };
}

template <typename Array, unsigned long Index>
constexpr inline Type ctarray_get_v = Private::get<Array, Index>::value;

template<typename Array1, typename Array2>
constexpr inline bool ctarray_cmp_v = Private::compare<Array1, Array2>::value;