
// Copyright (c) Matthew Brecknell 2012.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or a copy at http://www.boost.org/LICENSE_1_0.txt).

#include <type_traits>

// is_value<T>
// -----------
// Definition: A type `T` is a value type iff `const T` is immutable.
//
// Specialisation: is_value<T> may be specialised for user-defined types,
// though care is needed to ensure that candidate types correctly propagate
// const-ness to all subobjects, no matter how subobjects are accessed.

template <typename T, typename Enable = void>
struct is_value : std::false_type {};

// immutable<T>
// ------------
// Definition: A type `T` is immutable if and only if the observable behaviour
// of any object of type `T` is invariant over the lifetime of that object.
//
// Specialisation: immutable<T> may be specialised; however, the default case
// is implemented in terms of is_value<T>, so is preferrable to specialise only
// is_value<T>.

template <typename T>
struct immutable
  : std::integral_constant <
      bool
    , std::is_const<T>::value
      && is_value<T>::value
    > {};

// Specialisations for is_value<T>.

template <typename T>
struct is_value <
    T
  , typename std::enable_if<
      std::is_fundamental<T>::value || 
      std::is_enum<T>::value
    >::type
  >
  : std::true_type {};

// is_value<T> ==> is_value<(container of T)>

template <typename T>
struct is_value <
    T
  , typename std::enable_if<
      // This is probably too broad. May need to enumerate container types,
      // or define auxilliary container_traits to decouple the enumeration.
      is_value<typename T::value_type>::value
    >::type
  >
  : std::true_type {};

