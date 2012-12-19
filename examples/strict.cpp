
// Copyright (c) Matthew Brecknell 2012.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or a copy at http://www.boost.org/LICENSE_1_0.txt).

#include <iostream>
#include <string>
#include "../include/strict.hpp"

// data MySum = One Int | Two String

struct one { int int_val; };
struct two { std::string string_val; };

typedef strict::sum<one,two> mysum;

int main() {
  mysum foo = one{21};
  mysum bar = two{"Boo!"};
  mysum baz = bar;

  for (const mysum &i: {foo, bar, baz}) {
    std:: cout
      << i.match <std::string> (
        [](const one & x) { return "Got one! " + std::to_string(x.int_val); },
        [](const two & y) { return "Got two! " + y.string_val; },
        []() { return "Default!"; },
        99
      )
      << std::endl;
  }
}

