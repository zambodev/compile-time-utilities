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

static_assert(ctarray_cmp_v<
              ctarray_sort_t<int, ctarray<int, 3, 5, 1, 2, 4>>,
              ctarray<int, 1, 2, 3, 4, 5>
              >);

int main(void)
{
    using map = Map<int, 16,
        Pair<int>(Cstring<64>("ciao"), 10),
        Pair<int>(Cstring<64>("helo"), 9)
    >;

    for (auto item : ctarray<int, 1, 2, 3, 4>::arr)
    {
        std::cout << "Item: " << item << "\n";
    }

    for (unsigned long i = 0; i < map::arr.size(); ++i)
    {
        std::cout << map::arr[i].first.get_crc32() << " " << map::arr[i].second << "\n";
    }
}
