
// Copyright (c) Matthew Brecknell 2012.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or a copy at http://www.boost.org/LICENSE_1_0.txt).

#include <iostream>
#include <string>
#include "tree.hpp"

typedef tree<int> tri;

int main() {

  tri foo = tri(tri(tri(1), 2, tri(3)), 4, tri(tri(5), 6, tri()));
  tri bar = insert(7, foo);

  for (const tri & tr: {foo,bar}) {
    std::cout << tr << std::endl;
  }

  for (const tri & tr: {foo,bar}) {
    std::cout
      << map_tree<const int*>([](const int & i) { return &i; }, tr)
      << std::endl;
  }

}

