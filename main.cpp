#include <iostream>

#include "ctarray.hpp"
#include "ctmap.hpp"

static_assert(
    ctu::ctarray_cmp_v<ctu::array<int, 1, 2, 3>, ctu::array<int, 1, 2, 3>>);
static_assert(ctu::ctarray_get_v<ctu::array<int, 1, 2, 3>, 2> == 3);
static_assert(
    ctu::ctarray_cmp_v<ctu::ctarray_drop_t<ctu::array<int, 1, 2, 3>, 2>,
                       ctu::array<int, 3>>);
static_assert(
    ctu::ctarray_cmp_v<ctu::ctarray_prepend_t<int, 1, ctu::array<int, 2, 3>>,
                       ctu::array<int, 1, 2, 3>>);
static_assert(
    ctu::ctarray_cmp_v<ctu::ctarray_take_t<ctu::array<int, 1, 2, 3>, 2>,
                       ctu::array<int, 1, 2>>);
static_assert(ctu::ctarray_get_v<ctu::array<int, 1, 2, 3>, 1> == 2);
static_assert(ctu::ctarray_search_v<int, ctu::array<int, 1>, 1> == 0);
static_assert(
    ctu::ctarray_cmp_v<ctu::ctarray_fit_t<int, ctu::array<int, 3, 4>, 2>,
                       ctu::array<int, 0, 0, 3, 4>>);
static_assert(
    ctu::ctarray_cmp_v<ctu::ctarray_norm_t<int, ctu::array<int, 12, 13, 14>, 8>,
                       ctu::array<int, 4, 5, 6>>);
static_assert(
    ctu::ctarray_doubles_v<int, ctu::array<int, 1, 2, 3, 4, 4, 5, 6>> == true);
static_assert(
    ctu::ctarray_cmp_v<ctu::ctarray_concat_t<int, ctu::array<int, 1, 2, 3>,
                                             ctu::array<int, 4, 5, 6>>,
                       ctu::array<int, 1, 2, 3, 4, 5, 6>>);
static_assert(
    ctu::ctarray_cmp_v<ctu::ctarray_sort_t<int, ctu::array<int, 3, 5, 1, 2, 4>>,
                       ctu::array<int, 1, 2, 3, 4, 5>>);

int main(void) {
  ctu::ctmap<int, 8, {"ciao", 10}, {"helo", 9}, {"hallo", 2}> cmap;

  std::cout << "Value of 'hallo': " << cmap["hallo"] << "\n";
}
