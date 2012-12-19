
// Copyright (c) Matthew Brecknell 2012.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or a copy at http://www.boost.org/LICENSE_1_0.txt).

#include "include/list.hpp"

int main() {
  std::cout << "\nList build by cons-truction:\n\n  ws = ";
  list<int> ws = cons(0, cons(1, cons(2, cons(3, list<int>()))));

  std::cout << ws << "\n\n";

  std::cout << "List built from an initializer_list:\n\n  xs = ";
  list<int> xs = {0,1,2,3};

  std::cout << xs << "\n\n";

  std::cout << "List built by composition of reverse, map, and filter:\n\n  ys = ";
  auto ys =
    reverse(
      map<int>(
        filter(xs, [](int x) { return x%2; }),
        [](int x) { return x*10; }
      )
    );

  std::cout << ys << "\n\n";

  std::cout << "List built by concatenation:\n\n  zs = ";
  auto zs = concat(ys,xs);

  std::cout << zs << "\n\n";

  std::cout << "Pointers to elements, showing sharing between lists:\n\n";
  auto ps =
    map<list<int*>>(
      list<list<int>> {ws,xs,ys,zs},
      [](list<int> & xs) {
        return map<int*>(
          xs,
          [](int & x) { return &x; }
        );
      }
    );

  ps.foreach(
    [](list<int*> & ps) {
      std::cout << "  " << ps << '\n';
    }
  );

  std::cout
    << "\nComparisons:\n"
    << "\n  ws == xs: " << (ws == xs)
    << "\n  ws <= xs: " << (ws <= xs)
    << "\n  ws <  xs: " << (ws <  xs)
    << "\n  ys == zs: " << (ys == zs)
    << "\n  ys <= zs: " << (ys <= zs)
    << "\n  ys <  zs: " << (ys <  zs)
    << "\n\n";
}

