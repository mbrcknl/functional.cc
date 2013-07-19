
// Copyright (c) Matthew Brecknell 2013.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or a copy at http://www.boost.org/LICENSE_1_0.txt).

#ifndef FUNCTIONAL_CPP_INCLUDED_FP_UNIT_HPP
#define FUNCTIONAL_CPP_INCLUDED_FP_UNIT_HPP

// A family of standard unit types.

// Boxed sum types are specialised for these unit types, to reduce useless
// pointer chasing.

template <typename T = void>
struct tag {};

template <typename T>
bool operator == (const tag<T> & x, const tag<T> & y) {
  return true;
}

template <typename T>
bool operator != (const tag<T> & x, const tag<T> & y) {
  return false;
}

template <typename T>
bool operator <= (const tag<T> & x, const tag<T> & y) {
  return true;
}

template <typename T>
bool operator >= (const tag<T> & x, const tag<T> & y) {
  return true;
}

template <typename T>
bool operator < (const tag<T> & x, const tag<T> & y) {
  return false;
}

template <typename T>
bool operator > (const tag<T> & x, const tag<T> & y) {
  return false;
}

struct unit : tag<unit> {};
struct nil  : tag<nil>  {};

#endif

