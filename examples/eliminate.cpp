
#include <iostream>
#include <fp/eliminate.hpp>

using fp::_;

struct test {

  test(int i) : i(i) {}

  using elim = fp::eliminate<
    _(),
    _(int),
    _(const char *),
    _(int,int)
    >;

  template <typename Ret = _, typename... Funcs>
  typename fp::eliminate_result<elim,Ret,Funcs...>::type
  eliminate(Funcs &&... funcs) {
    auto e = elim::with<Ret>(std::forward<Funcs>(funcs)...);
    switch(i) {
      case 1: return e(1);
      case 2: return e("Hello!");
      case 3: return e(2,3);
    }
    return e();
  }

  int i;

  // Compile-time check that calls to the eliminator are valid,
  // by forcing instantiation of test::eliminate.
  // TODO: figure out if we can do this without any extra effort.

  void check() { eliminate(elim::check()); }

};

struct src1 {
  src1(int i) : i(i) {};
  int i;
};

struct src2 {
  src2(int i) : i(i) {};
  int i;
};

struct dst {
  dst(src1 s) : i(s.i) {}
  dst(src2 s) : i(s.i) {}
  int i;
  void foo() { std::cout << "dst:" << i << "\n"; }
};

int main() {

  for (int i=0; i<4; ++i) {
    test t(i);
    t.eliminate<dst>(
      []() { std::cout << "empty\n"; return src1(0); },
      [](const char * msg) { std::cout << msg << "\n"; return src2(2); },
      [](int x) { std::cout << x << "\n"; return src1(1); },
      [](long x, long y) { std::cout << x << " " << y << "\n"; return src2(3); }
      // [](_) { std::cout << "wow!\n"; return src1(-1); }
    ).foo();

    std::cout << (t.eliminate(test::elim::check()) == i ? "check OK\n" : "check FAIL\n");
  }

}

