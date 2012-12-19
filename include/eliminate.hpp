
// Copyright (c) Matthew Brecknell 2012.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or a copy at http://www.boost.org/LICENSE_1_0.txt).

#ifndef FUNCTIONAL_CPP_INCLUDE_ELIMINATE_HPP
#define FUNCTIONAL_CPP_INCLUDE_ELIMINATE_HPP

#include <utility>

namespace fp {

  template <typename... Specs>
  class eliminate {

    // Check that each Specs... is of the form T(Args...)

  public:

    template <typename... Funcs>
    class eliminate_with {

      template <typename... Args>
      struct dispatch_result;

    public:

      // typedef ... result_type;

      template <typename... Args>
      // SFINAE to disable if Args don't match?
      // or do we just return result_type, and defer the error?
      // SFINAE would give more sensible error messages.
      typename dispatch_result<Args...>::type
      operator()(Args &&... args) {

      }

    };

    template <typename... Funcs>
    eliminate_with<Funcs...> with(Funcs &&... funs) {
      return eliminate_with<Funcs...>(std::forward<Funcs>(funcs)...);
    }

  };


  // ------------------------------

  template <typename... Specs> struct eliminate;

  template <typename Cons, typename... Args, typename... Specs>
  struct eliminate <Cons(Args...), Specs...> {

  };

  namespace detail {

  }

  template <typename Match> struct eliminate_result;

  template <typename... Specs, typename F, typename... Funcs>
  struct eliminate_result <eliminate<Specs...>(F,Funcs...)>
    : detail::

  template <typename... Specs, typename... Funcs>
  struct eliminate_result <eliminate<Specs...>(Funcs...)>
    : detail::eliminate_result <eliminate<Specs...>(Funcs...)> {};

}

#endif // FUNCTIONAL_CPP_INCLUDE_ELIMINATE_HPP
