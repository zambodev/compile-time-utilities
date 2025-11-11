#include <ctu/ctarray.hpp>
#include <ctu/ctmap.hpp>
#include <iostream>

static_assert(
    ctu::ctarray_cmp_v<ctu::ctarray<int, 1, 2, 3>, ctu::ctarray<int, 1, 2, 3>>);
static_assert(ctu::ctarray_get_v<ctu::ctarray<int, 1, 2, 3>, 2> == 3);
static_assert(
    ctu::ctarray_cmp_v<ctu::ctarray_drop_t<ctu::ctarray<int, 1, 2, 3>, 2>,
                       ctu::ctarray<int, 3>>);
static_assert(
    ctu::ctarray_cmp_v<ctu::ctarray_prepend_t<int, 1, ctu::ctarray<int, 2, 3>>,
                       ctu::ctarray<int, 1, 2, 3>>);
static_assert(
    ctu::ctarray_cmp_v<ctu::ctarray_take_t<ctu::ctarray<int, 1, 2, 3>, 2>,
                       ctu::ctarray<int, 1, 2>>);
static_assert(ctu::ctarray_get_v<ctu::ctarray<int, 1, 2, 3>, 1> == 2);
static_assert(ctu::ctarray_search_v<int, ctu::ctarray<int, 1>, 1> == 0);
static_assert(
    ctu::ctarray_cmp_v<ctu::ctarray_fit_t<int, ctu::ctarray<int, 3, 4>, 2>,
                       ctu::ctarray<int, 0, 0, 3, 4>>);
static_assert(ctu::ctarray_cmp_v<
              ctu::ctarray_norm_t<int, ctu::ctarray<int, 12, 13, 14>, 8>,
              ctu::ctarray<int, 4, 5, 6>>);
static_assert(
    ctu::ctarray_doubles_v<int, ctu::ctarray<int, 1, 2, 3, 4, 4, 5, 6>> ==
    true);
static_assert(
    ctu::ctarray_cmp_v<ctu::ctarray_concat_t<int, ctu::ctarray<int, 1, 2, 3>,
                                             ctu::ctarray<int, 4, 5, 6>>,
                       ctu::ctarray<int, 1, 2, 3, 4, 5, 6>>);
static_assert(ctu::ctarray_cmp_v<
              ctu::ctarray_sort_t<int, ctu::ctarray<int, 3, 5, 1, 2, 4>>,
              ctu::ctarray<int, 1, 2, 3, 4, 5>>);

void func(void) { std::cout << "ok\n"; }
void func2(void) { std::cout << "ok 2\n"; }

int main(void) {
  constexpr ctu::ctmap<ctu::ctstring<64>, int, {"ciao", 10}, {"helo", 9},
                       {"hallo", 2}>
      cmap;

  cmap.print();
  std::cout << "Value of 'hallo': " << cmap["hallo"] << "\n";

  //   constexpr ctu::ctmap<int, void (*)(), 8, {1324234213, func}, {3789987,
  //   func2},
  //                        {213235689,
  //                         [](void) -> void { std::cout << "lambda\n"; }}>
  //       cmap2;

  //   cmap2[1324234213]();
  //   cmap2[3789987]();
  //   cmap2[213235689]();
}
