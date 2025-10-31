#include <iostream>
#include "ctarr_utils.hpp"
#include "ctmap.hpp"

static_assert(ctarray_cmp_v<ctarray<int, 1,2,3>, ctarray<int, 1,2,3>>);
static_assert(ctarray_get_v<ctarray<int, 1,2,3>, 2> == 3);

static_assert(ctarray_cmp_v<ctarray_drop_t<ctarray<int, 1,2,3>, 2>, ctarray<int, 3>>);
static_assert(ctarray_cmp_v<ctarray_prepend_t<int, 1, ctarray<int, 2, 3>>, ctarray<int, 1, 2, 3>>);
static_assert(ctarray_cmp_v<ctarray_take_t<ctarray<int, 1,2,3>, 2>, ctarray<int, 1,2>>);
static_assert(ctarray_get_v<ctarray<int, 1,2,3>, 1> == 2);
static_assert(ctarray_search_v<int, ctarray<int, 1>, 1> == 0);
static_assert(ctarray_cmp_v<ctarray_fit_t<int, ctarray<int, 3, 4>, 2>, ctarray<int, 0, 0, 3, 4>>);
static_assert(ctarray_cmp_v<ctarray_norm_t<int, ctarray<int, 12, 13, 14>, 8>, ctarray<int, 4, 5, 6>>);

static_assert(ctarray_cmp_v<
              ctarray_sort_t<int, ctarray<int, 3, 5, 1, 2, 4>>,
              ctarray<int, 1, 2, 3, 4, 5>
              >);


template <typename Type, Type I0>
auto test(unsigned long val)
{
    return I0 % val;
}


int main(void)
{
    // using map = Map<int, 16,
    //     Pair("ciao", 10),
    //     Pair("helo", 9)
    // >;

    using arr = ctarray_norm_t<Pair<int>,
        ctarray<Pair<int>, Pair("ciao", 2), Pair("hello", 3)>,
        26>;

    for (unsigned long i = 0; i < arr::arr.size(); ++i)
    {
        std::cout << arr::arr[i].crc << " " << arr::arr[i].second << "\n";
    }

    auto p = test<Pair<int>, Pair<int>("ciao", 2)>(4);
    std::cout << p.crc << "\n";
}
