
// Copyright (c) Matthew Brecknell 2012.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or a copy at http://www.boost.org/LICENSE_1_0.txt).

#ifndef FUNCTIONAL_CPP_INCLUDED_FP_ELIMINATE_HPP
#define FUNCTIONAL_CPP_INCLUDED_FP_ELIMINATE_HPP

#include <type_traits>
#include <utility>

#include "call_traits.hpp"
#include "meta/variadic.hpp"

namespace fp {

  template <typename... Specs>
  struct eliminate;

  class eliminate_impl {

    template <typename... Specs>
    friend class eliminate;

    // Check that each Specs... is of the form T(Args...).

    template <typename... Specs>
    struct check_eliminate_args : std::true_type {};

    template <typename Spec, typename... Specs>
    struct check_eliminate_args <Spec,Specs...>
      : std::integral_constant<
          bool,
          std::is_function<Spec>::value &&
          check_eliminate_args<Specs...>::value
        > {};

    // Calculate the result type for an elimination.

    template <typename Func, typename... Specs>
    struct elim_with_one;

    template <typename Funcs, typename Specs, typename T>
    struct elim_with;

    template <typename Func, typename... Funcs, typename... Specs, typename T>
    struct elim_with <void(Func,Funcs...), void(Specs...), T> {

    };

    template <typename Spec, typename... Specs, typename T>
    struct elim_with <void(), void(Spec,Specs...), T> {
      static const bool has_result_type = false;
    };

    template <typename T>
    struct elim_with <void(), void(), T> {

    };

    // Dispatch calls to eliminator.

    template <typename Args, typename Funcs, typename Specs>
    struct eliminate_dispatch;

  };

  template <typename... Specs>
  struct eliminate {

    // Check that each Specs... is of the form T(Args...).

    static_assert(
      eliminate_impl::check_eliminate_args<Specs...>::value,
      "fp::eliminate<...> template parameters must each be of the form T(Args...)"
    );

    template <typename... Funcs>
    struct eliminate_with {

      // TODO: move to eliminate_impl
      template <typename... Args>
      struct dispatch;

      // typedef ... result_type;

      template <typename... Args>
      // SFINAE to disable if Args don't match?
      // or do we just return result_type, and defer the error?
      // or use static_assert with an explicit error message?
      // SFINAE would give reasonably sensible error messages.
      // static_assert might give better error messages, but difficult to add trace info.
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

