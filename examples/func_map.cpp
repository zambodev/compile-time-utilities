#include <ctu/ctarray.hpp>
#include <ctu/ctmap.hpp>
#include <iostream>

void func(void) { std::cout << "ok\n"; }
void func2(void) { std::cout << "ok 2\n"; }

int main(void) {
  ctu::ctmap<int, void (*)(), {1324234213, func}, {3789987, func2},
             {213235689, [](void) -> void { std::cout << "lambda\n"; }}>
      cmap;

  
  if (auto exp = cmap[1324234213]; exp.has_value())
    exp.value()();
  if (auto exp = cmap[3789987]; exp.has_value())
    exp.value()();
  if (auto exp = cmap[213235689]; exp.has_value())
    exp.value()();
}

/* Output:
ok
ok 2
lambda
*/
