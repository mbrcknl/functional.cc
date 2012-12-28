
// Copyright (c) Matthew Brecknell 2012.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or a copy at http://www.boost.org/LICENSE_1_0.txt).

#include <iostream>
#include <string>
#include <typeinfo>

#include "../include/fp/call_traits.hpp"

template <typename F>
void blah(F && f) {
  std::cout
    << typeid(typename fp::callable_traits<F>::param_types).name()
    << std::endl;
}

void foo(const char *) {};

int main() {
  blah([](int i) { return i+1; });
  void (*const bar)(const char *) = foo;
  blah(bar);
}

