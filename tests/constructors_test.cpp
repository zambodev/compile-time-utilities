#include <ctu/ctpair.hpp>
#include <ctu/ctstring.hpp>
#include <iostream>

int main(void) {
  ctu::ctstring<64> str("ciao");
  ctu::ctstring<64> str2(str);
  ctu::ctstring<64> str3(std::move(str));

  ctu::ctpair<int, int> pair(1, 2);
  ctu::ctpair<int, int> pair2(pair);
  ctu::ctpair<int, int> pair3(std::move(pair));

  std::cout << str2.c_str << " " << str3.c_str << "\n";
  std::cout << pair2.second << " " << pair3.second << "\n";
}
