
// Copyright (c) Matthew Brecknell 2012.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or a copy at http://www.boost.org/LICENSE_1_0.txt).

// option<T>
// ---------
// An immutable, strict, unboxed, optional value type.

#include <type_traits>
#include <utility>

#include "call_traits.hpp"

template <typename T>
class option final {

  // Union reserves properly-aligned space, but does not automatically
  // construct or destruct value (C++11).

  union { T value; };
  const bool full;

  const T& const_value() const { return value; }

  struct some_tag {};

  template <typename... Args>
  explicit option(some_tag, Args &&... args)
    : full(true), value(std::forward<Args>(args)...) {}

  // Not sure why this is necessary (GCC 4.7.0).
  explicit option(some_tag) : full(true), value() {}

public:

  option() : full(false) {}

  option(const option & other) : full(other.full) {
    if (full) { new (&value) T(other.value); }
  }

  option(option && other) : full(other.full) {
    if (full) { new (&value) T(std::move(other.value)); }
  }

  option & operator = (const option &) = delete;
  option & operator = (option &&) = delete;

  option(const T & value)
    : full(true), value(value) {}

  option(T && value)
    : full(true), value(std::move(value)) {}

  operator bool() { return full; }

  // Factory function which constructs the value in-place.

  template <typename... Args>
  static option some(Args &&... args) {
    return option(some_tag(), std::forward<Args>(args)...);
  }

  // Perform a computation: option<T> ==> R,
  //
  // Arguments:
  // - f :: T ==> R, in case the option<T> contains a value,
  // - n ::   ==> R, in case the option<T> is empty.

  template <typename F, typename N>
  typename
    std::common_type<
      typename result_of<F(T)>::type,
      typename result_of<N()>::type
    >::type
  operator()(F && f, N && n) const {
    return full ? f(const_value()) : n();
  }

  // Perform a computation: option<T> ==> R,
  //
  // Argument:
  // - f :: T ==> R, in case the option<T> contains a value.
  //
  // Requirement:
  // - R must be default-constructible, in case the option<T> is empty.
  //
  // Useful for chaining computations on optional values.

  template <typename F>
  typename result_of<F(T)>::type
  operator >> (F && f) const {
    return full ? f(const_value()) : typename result_of<F(T)>::type();
  }

  ~option() {
    if (full) { value.~T(); }
  }

};

// Convenient construction using conventional names.

template <typename T, typename... Args>
option<T> some(Args &&... args) {
  return option<T>::some(std::forward<Args>(args)...);
}

template <typename T>
option<T> none() {
  return option<T>();
}

