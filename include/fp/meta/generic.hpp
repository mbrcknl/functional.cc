
// Copyright (c) Matthew Brecknell 2012.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or a copy at http://www.boost.org/LICENSE_1_0.txt).

#ifndef FUNCTIONAL_CPP_INCLUDED_FP_META_GENERIC_HPP
#define FUNCTIONAL_CPP_INCLUDED_FP_META_GENERIC_HPP

namespace fp {

  namespace meta {

    // Apply a type-level function: a struct with member template "type::apply_".

    template <typename F, typename... Args>
    struct apply : F::type::template apply_<Args...> {};

    // Wrap a template struct to make a type-level function.
    // Assumes the template struct gives its result via a member typedef.

    template <template <typename...> class F>
    struct fun {

      template <typename... Args>
      struct apply_ : F<Args...> {};

      typedef fun type;

    };

    // Generic operations on type-level structures.

    template <typename T, typename... Destr>
    struct elim : T::type::template elim_<Destr...> {};

    template <typename T, typename... Fold>
    struct fold : T::type::template fold_<Fold...> {};

  }

}

#endif

