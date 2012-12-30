
// Copyright (c) Matthew Brecknell 2012.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or a copy at http://www.boost.org/LICENSE_1_0.txt).

#ifndef FUNCTIONAL_CPP_INCLUDED_FP_ELIMINATE_HPP
#define FUNCTIONAL_CPP_INCLUDED_FP_ELIMINATE_HPP

#include <type_traits>
#include <utility>

#include "call_traits.hpp"
#include "meta/list.hpp"
#include "meta/option.hpp"

namespace fp {

  template <typename... Specs>
  struct eliminate;

  namespace impl {

    // Check that each Specs... is of the form T(Args...).

    template <typename... Specs>
    struct check_eliminate_args;

    template <typename Spec, typename... Specs>
    struct check_eliminate_args <Spec,Specs...>
      : std::integral_constant<
          bool,
          std::is_function<Spec>::value &&
          check_eliminate_args<Specs...>::value
        > {};

    template <>
    struct check_eliminate_args <> : std::true_type {};

    // Calculate the result type for an elimination.

    template <typename Func, typename SpecsList>
    struct elim_with_one;

    template <typename FuncsList, typename SpecsList, typename T>
    struct elim_with;

    template <typename Func, typename... Funcs, typename SpecsList, typename T>
    struct elim_with <meta::list<Func,Funcs...>, SpecsList, T>
      : meta::elim<
          typename elim_with_one<Func,SpecsList>::result_types,
          meta::option<>, // TODO!
          meta::option<> // Func has no match in SpecsList.
        > {};

    template <typename Spec, typename... Specs, typename T>
    struct elim_with <meta::list<>, meta::list<Spec,Specs...>, T>
      : meta::option<> {};

    template <typename T>
    struct elim_with <meta::list<>, meta::list<>, T>
      : T {};

    // Dispatch calls to eliminator.

    template <typename Args, typename Funcs, typename Specs>
    struct eliminate_dispatch;

  }

  template <typename... Specs>
  struct eliminate {

    // Check that each Specs... is of the form T(Args...).

    static_assert(
      impl::check_eliminate_args<Specs...>::value,
      "fp::eliminate<...> template parameters must each be of the form T(Args...)"
    );

    template <typename... Funcs>
    struct eliminate_with {

      // TODO: move to impl
      template <typename... Args>
      struct dispatch;

      // typedef ... result_type;

      template <typename... Args>
      // SFINAE to disable if Args don't match?
      // or do we just return result_type, and defer the error?
      // or use static_assert with an explicit error message?
      // SFINAE would give reasonably sensible error messages.
      // static_assert might give better error messages,
      // but difficult to add trace info.
      typename dispatch<Args...>::result_type
      operator()(Args &&... args) {

      }

    };

    template <typename... Funcs>
    eliminate_with<Funcs...> with(Funcs &&... funcs) {
      return eliminate_with<Funcs...>(std::forward<Funcs>(funcs)...);
    }

  };

  template <typename T> struct eliminate_result;

  template <typename E, typename... Funcs>
  struct eliminate_result <E(Funcs...)> {
    typedef typename E::template eliminate_with<Funcs...>::result_type type;
  };

}

#endif

