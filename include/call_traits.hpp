
// Copyright (c) Matthew Brecknell 2012.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or a copy at http://www.boost.org/LICENSE_1_0.txt).

#ifndef MASALA_INCLUDED_CALL_TRAITS_HPP
#define MASALA_INCLUDED_CALL_TRAITS_HPP

#include <type_traits>

#include "variadic.hpp"

namespace fp {

  // Static test whether a type is callable with given argument types.

  template <typename> struct is_callable_with;

  template <typename Fun, typename... Args>
  class is_callable_with <Fun(Args...)> {

    template <typename F> static std::true_type
      test(decltype(std::declval<F>()(std::declval<Args>()...)) *);

    template <typename F> static std::false_type
      test(...);

  public:

    typedef bool value_type;
    typedef is_callable_with type;

    static const bool value = decltype(test<Fun>(nullptr))::value;

    constexpr operator value_type() const { return value; }

  };

  // Determine result type of a call, if arguments are compatible.

  template <typename Call, typename Enable = void>
  struct result_of;

  template <typename Fun, typename... Args>
  struct result_of
    <
      Fun(Args...),
      typename std::enable_if<is_callable_with<Fun(Args...)>::value>::type
    >
    : std::result_of<Fun(Args...)> {};

  template <typename Fun, typename... Args>
  struct result_of
    <
      Fun(Args...),
      typename std::enable_if<!is_callable_with<Fun(Args...)>::value>::type
    >
    {};

  // Determine return type and parameter types for a callable type.

  namespace impl {

    // Test for a monomorphic (non-overloaded, non-template) operator().

    template <typename T>
    class has_call_operator {

      template <typename F> static std::true_type
        test(decltype(&F::operator()));

      template <typename F> static std::false_type
        test(...);

    public:

      typedef bool value_type;
      typedef has_call_operator type;

      static const bool value = decltype(test<T>(nullptr))::value;

      constexpr operator value_type() const { return value; }

    };

    // Determine return type and parameter types for a member function type.

    template <typename T>
    struct memfun_traits;

    template <typename Ret, typename T, typename... Args>
    struct memfun_traits <Ret(T::*)(Args...)> {
      typedef Ret return_type;
      typedef pack<Args...> param_types;
    };

    template <typename Ret, typename T, typename... Args>
    struct memfun_traits <Ret(T::*)(Args...)const> {
      typedef Ret return_type;
      typedef pack<Args...> param_types;
    };

    // Determine return type and parameter types for a callable type.

    template <typename F, typename Enable = void>
    struct callable_traits;

    template <typename Ret, typename... Args>
    struct callable_traits <Ret(Args...)> {
      typedef Ret return_type;
      typedef pack<Args...> param_types;
    };

    template <typename Ret, typename... Args>
    struct callable_traits <Ret(*)(Args...)> {
      typedef Ret return_type;
      typedef pack<Args...> param_types;
    };

    template <typename F>
    struct callable_traits
      <F,typename std::enable_if<has_call_operator<F>::value>::type>
      : memfun_traits<decltype(&F::operator())> {};

  }

  // Determine return type and parameter types for a callable type.

  template <typename F>
  struct callable_traits
    : impl::callable_traits<
        typename std::remove_cv<
          typename std::remove_reference<F>::type
        >::type
      > {};

}

#endif // MASALA_INCLUDED_CALL_TRAITS_HPP

