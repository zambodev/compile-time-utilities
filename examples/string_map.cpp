#include <ctu/ctarray.hpp>
#include <ctu/ctmap.hpp>
#include <iostream>

int main(void) {
  constexpr ctu::ctmap<ctu::ctstring<64>, int, {"ciao", 10}, {"halo", 9},
                       {"hello", 2}>
      cmap;

  cmap.print();
  std::cout << "Value of 'hello': " << cmap["hello"] << "\n";
}

/* Output:
[0] = 0
[1] = 10
[2] = 9
[3] = 2
Value of 'hello': 2
*/
