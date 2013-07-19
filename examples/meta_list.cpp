
// Copyright (c) Matthew Brecknell 2013.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or a copy at http://www.boost.org/LICENSE_1_0.txt).

#include <iomanip>
#include <iostream>
#include <type_traits>
#include <fp/meta/list.hpp>

template <int i>
struct ic : std::integral_constant<int,i> {};

namespace m = fp::meta;

typedef m::join<
  m::list<
    m::list<ic<0x0>,ic<0x1>,ic<0x2>,ic<0x3>>,
    m::list<ic<0x4>,ic<0x5>,ic<0x6>,ic<0x7>>,
    m::list<ic<0x8>,ic<0x9>,ic<0xA>,ic<0xB>>,
    m::list<ic<0xC>,ic<0xD>,ic<0xE>,ic<0xF>>
  >
>::type ex_list;

template <typename L>
struct foreach_list;

template <typename i, typename... T>
struct foreach_list <m::list<i,T...>> {

  template <typename F>
  static void foreach(const F & f) {
    f(i());
    foreach_list<m::list<T...>>::foreach(f);
  }

};

template <>
struct foreach_list <m::list<>> {
  template <typename F>
  static void foreach(const F & f) {}
};

int main() {
  foreach_list<ex_list>::foreach(
    [](int i) { std::cout << std::hex << i << '\n'; }
  );
}

