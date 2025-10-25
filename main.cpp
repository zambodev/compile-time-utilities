#include "ctarr_utils.hpp"

static_assert(ctarray_cmp_v<ctarray<int, 1,2,3>, ctarray<int, 1,2,3>>);
static_assert(ctarray_get_v<ctarray<int, 1,2,3>, 2> == 3);

static_assert(ctarray_cmp_v<ctarray_drop_t<ctarray<int, 1,2,3>, 2>, ctarray<int, 3>>);
static_assert(ctarray_cmp_v<ctarray_prepend_t<int, 1, ctarray<int, 2, 3>>, ctarray<int, 1, 2, 3>>);
static_assert(ctarray_cmp_v<ctarray_take_t<ctarray<int, 1,2,3>, 2>, ctarray<int, 1,2>>);
static_assert(ctarray_get_v<ctarray<int, 1,2,3>, 1> == 2);
static_assert(ctarray_search_v<int, ctarray<int, 1>, 1> == 0);

int main(void)
{
}