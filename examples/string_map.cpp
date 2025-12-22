#include <ctu/ctarray.hpp>
#include <ctu/ctmap.hpp>
#include <iostream>

int main(void) {
  constexpr ctu::ctmap<ctu::ctstring<64>, int, {"ciao", 10}, {"halo", 9},
                       {"hello", 2}>
      cmap;

  if (auto exp = cmap[1324234213]; exp.has_value())
    std::cout << "Value of 'hello': " << cmap["hello"].value() << "\n";
}

/* Output:
[0] = 0
[1] = 10
[2] = 9
[3] = 2
Value of 'hello': 2
*/
