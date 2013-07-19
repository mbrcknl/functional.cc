
// Copyright (c) Matthew Brecknell 2013.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or a copy at http://www.boost.org/LICENSE_1_0.txt).

#ifndef FUNCTIONAL_CC_INCLUDED_FP_CALL_TRAITS_HPP
#define FUNCTIONAL_CC_INCLUDED_FP_CALL_TRAITS_HPP

#include <type_traits>

#include <fp/meta/list.hpp>
#include <fp/meta/option.hpp>

namespace fp {

  // A re-implementation of std::common_type suitable for meta-programming.
  // Returns a meta::option<...> indicating whether the common type exists.

  template <typename... T>
  struct common_type;

  namespace impl {

    template <typename T, typename... U>
    struct common_type_opt;

    template <typename T, typename... V>
    struct common_type_opt <meta::option<T>, V...> : common_type<T,V...> {};

    template <typename... V>
    struct common_type_opt <meta::option<>, V...> : meta::option<> {};

  }

  template <typename T>
  struct common_type <T> : meta::option<T> {};

  template <typename T, typename U, typename V, typename... W>
  struct common_type <T,U,V,W...>
    : impl::common_type_opt<typename common_type<T,U>::type, V, W...> {};

  template <>
  struct common_type <void,void> : meta::option<void> {};

  template <typename T, typename U>
  class common_type <T,U> {

    template <typename S>
    static meta::option<typename std::remove_reference<
      decltype(true ? std::declval<S>() : std::declval<U>())
    >::type>
    test(S && s);

    static meta::option<> test(...);

  public:

    typedef decltype(test(std::declval<T>())) type;

  };

  template <typename TList>
  struct common_type_list : meta::unpack<meta::fun<common_type>,TList> {};

  // Static test whether a type is callable with given argument types.

  template <typename>
  struct is_callable_with;

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

  // A re-implementation of std::result_of suitable for meta-programming.
  // Returns a meta::option<...> indicating whether the result type exists.

  template <typename>
  struct result_of;

  template <typename Fun, typename... Args>
  class result_of <Fun(Args...)> {

    template <typename F>
    static meta::option<decltype(std::declval<F>()(std::declval<Args>()...))>
    test(F && f);

    static meta::option<> test(...);

  public:

    typedef decltype(test(std::declval<Fun>())) type;

  };

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
      typedef meta::list<Args...> param_types;
    };

    template <typename Ret, typename T, typename... Args>
    struct memfun_traits <Ret(T::*)(Args...)const> {
      typedef Ret return_type;
      typedef meta::list<Args...> param_types;
    };

    // Determine return type and parameter types for a callable type.

    template <typename F, typename Enable = void>
    struct callable_traits;

    template <typename Ret, typename... Args>
    struct callable_traits <Ret(Args...)> {
      typedef Ret return_type;
      typedef meta::list<Args...> param_types;
    };

    template <typename Ret, typename... Args>
    struct callable_traits <Ret(*)(Args...)> {
      typedef Ret return_type;
      typedef meta::list<Args...> param_types;
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

#endif

