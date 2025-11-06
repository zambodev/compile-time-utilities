#include <iostream>
#include "ctarr_utils.hpp"
#include "ctmap.hpp"

static_assert(ctmap::ctarray_cmp_v<ctmap::ctarray<int, 1, 2, 3>, ctmap::ctarray<int, 1, 2, 3>>);
static_assert(ctmap::ctarray_get_v<ctmap::ctarray<int, 1, 2, 3>, 2> == 3);
static_assert(ctmap::ctarray_cmp_v<ctmap::ctarray_drop_t<ctmap::ctarray<int, 1, 2, 3>, 2>, ctmap::ctarray<int, 3>>);
static_assert(ctmap::ctarray_cmp_v<ctmap::ctarray_prepend_t<int, 1, ctmap::ctarray<int, 2, 3>>, ctmap::ctarray<int, 1, 2, 3>>);
static_assert(ctmap::ctarray_cmp_v<ctmap::ctarray_take_t<ctmap::ctarray<int, 1, 2, 3>, 2>, ctmap::ctarray<int, 1, 2>>);
static_assert(ctmap::ctarray_get_v<ctmap::ctarray<int, 1, 2, 3>, 1> == 2);
static_assert(ctmap::ctarray_search_v<int, ctmap::ctarray<int, 1>, 1> == 0);
static_assert(ctmap::ctarray_cmp_v<ctmap::ctarray_fit_t<int, ctmap::ctarray<int, 3, 4>, 2>, ctmap::ctarray<int, 0, 0, 3, 4>>);
static_assert(ctmap::ctarray_cmp_v<ctmap::ctarray_norm_t<int, ctmap::ctarray<int, 12, 13, 14>, 8>, ctmap::ctarray<int, 4, 5, 6>>);
static_assert(ctmap::ctarray_doubles_v<int, ctmap::ctarray<int, 1, 2, 3, 4, 4, 5, 6>> == true);
static_assert(ctmap::ctarray_cmp_v<ctmap::ctarray_concat_t<int, ctmap::ctarray<int, 1, 2, 3>, ctmap::ctarray<int, 4, 5, 6>>,
                                   ctmap::ctarray<int, 1, 2, 3, 4, 5, 6>>);
static_assert(ctmap::ctarray_cmp_v<ctmap::ctarray_sort_t<int, ctmap::ctarray<int, 3, 5, 1, 2, 4>>,
                                   ctmap::ctarray<int, 1, 2, 3, 4, 5>>);

int main(void)
{
    ctmap::CTMap<int, 8,
                 {"ciao", 10},
                 {"helo", 9},
                 {"hallo", 2}>
        cmap;

    cmap.print();
    std::cout << "Value of 'hallo': " << cmap["hallo"] << "\n";

    constexpr auto arr = ctmap::ctarray_norm_t<int,
                                               ctmap::ctarray<int, 1134134134, 1314124124, 124124124>,
                                               64>::arr;
}
