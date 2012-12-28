
// Copyright (c) Matthew Brecknell 2012.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or a copy at http://www.boost.org/LICENSE_1_0.txt).

#include <iostream>
#include <type_traits>
#include "../include/fp/meta/list.hpp"

template <int i>
struct ic : std::integral_constant<int,i> {};

typedef fp::meta::list<ic<0>,ic<1>> A;
typedef fp::meta::list<ic<2>,ic<3>> B;
typedef typename fp::meta::concat<A,B>::type C;

template <typename L>
struct foreach_list;

template <typename i, typename... T>
struct foreach_list <fp::meta::list<i,T...>> {

  template <typename F>
  static void foreach(const F & f) {
    f(i());
    foreach_list<fp::meta::list<T...>>::foreach(f);
  }

};

template <>
struct foreach_list <fp::meta::list<>> {
  template <typename F>
  static void foreach(const F & f) {}
};

int main() {
  foreach_list<C>::foreach(
    [](int i) { std::cout << i << '\n'; }
  );
}

