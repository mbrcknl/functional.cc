
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

    template <typename... TS>
    struct check_well_formed_specs : std::true_type {};

    template <typename T, typename... TS>
    struct check_well_formed_specs <T, TS...>
      : std::integral_constant<
          bool,
          std::is_function<T>::value &&
          check_well_formed_specs<TS...>::value
        > {};

    static_assert(
      check_well_formed_specs<Specs...>::value,
      "fp::eliminate<...> template parameters must each be of the form T(Args...)"
    );

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
    eliminate_with<Funcs...> with(Funcs &&... funcs) {
      return eliminate_with<Funcs...>(std::forward<Funcs>(funcs)...);
    }

  };

}

#endif // FUNCTIONAL_CPP_INCLUDE_ELIMINATE_HPP
