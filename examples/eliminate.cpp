
#include <iostream>
#include "../include/fp/eliminate.hpp"

struct test {

  test(int i) : i(i) {}

  using elim = fp::eliminate<
    void(),
    void(int),
    void(const char *),
    void(int,int)
    >;

  template <typename... Funcs>
  typename fp::eliminate_result<elim(Funcs...)>::type
  eliminate(Funcs &&... funcs) {
    auto e = elim::with(std::forward<Funcs>(funcs)...);
    switch(i) {
      case 0: return e();
      case 1: return e(1);
      case 2: return e("Hello!");
      case 3: return e(2,3);
    }
  }

  int i;

};

int main() {

  for (int i=0; i<4; ++i) {
    test t(i);
    t.eliminate(
      []() { std::cout << "empty\n"; },
      [](const char * msg) { std::cout << msg << "\n"; },
      [](int x) { std::cout << x << "\n"; },
      [](long x, long y) { std::cout << x << " " << y << "\n"; }
    );
  }

}

