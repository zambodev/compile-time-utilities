#include "ctarr_utils.hpp"

static_assert(ctarray_cmp_v<ctarray<1,2,3>, ctarray<1,2,3>>);
static_assert(ctarray_get_v<ctarray<1,2,3>, 2> == 3);

// static_assert(arr_cmp_v<drop<ctarray<1,2,3>, 2>::type, ctarray<3>>);
// static_assert(arr_cmp_v<prepend<1, ctarray<2, 3>>::type, ctarray<1, 2, 3>>);
// static_assert(arr_cmp_v<take<ctarray<1,2,3>, 2>::type, ctarray<1,2>>);
// static_assert(ctarray_get_v<ctarray<1,2,3>, 1>::value == 2);
// static_assert(search<ctarray<1,2,3,4,5>, 4>::value == 3);

int main(void)
{

}