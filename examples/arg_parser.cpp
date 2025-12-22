#include <ctu/ctarray.hpp>
#include <ctu/ctmap.hpp>
#include <iostream>

int main(int argc, char **argv) {
  constexpr ctu::ctmap<ctu::ctstring<64>, int, {"-t", 10}, {"-h", 0}> cmap;
  
  for (int i = 1; i < argc; ++i) {
    if (auto exp = cmap[argv[i]]; exp.has_value())
      std::cout << "[" << i << "] = " << exp.value() << "\n";
  }
}
