
#include <iostream>
#include "../include/fp/eliminate.hpp"

struct test {

  test(int i) : i(i) {}

  typedef fp::eliminate<int(int),int(int,int),int(long)> elim;

  template <typename... Funcs>
  typename fp::eliminate_result<elim(Funcs...)>::type
  eliminate(Funcs &&... funcs) {
    auto e = elim::with(std::forward<Funcs>(funcs)...);
    switch(i) {
      case 0: return e(0);
      case 1: return e(1,1);
      case 2: return e(2L);
    }
  }

  int i;

};

int main() {

  for (int i=0; i<3; ++i) {

    test t(i);

    std::cout
      << t.eliminate(
          [](int x) { return x; },
          [](int x, int y) { return x + y + 100; },
          [](long x) { return x + 200; }
        )
      << '\n';

  }

}

