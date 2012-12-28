
// Copyright (c) Matthew Brecknell 2012.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or a copy at http://www.boost.org/LICENSE_1_0.txt).

#ifndef FUNCTIONAL_CPP_INCLUDED_FP_META_OPTION_HPP
#define FUNCTIONAL_CPP_INCLUDED_FP_META_OPTION_HPP

#include "generic.hpp"

namespace fp {

  namespace meta {

    // An optional type.

    template <typename... T>
    struct option;

    template <typename T>
    struct option <T> {

      template <typename F, typename Z>
      struct elim_ : apply<F,T> {};

      typedef option type;

    };

    template <>
    struct option <> {

      template <typename F, typename Z>
      struct elim_ : Z {};

      typedef option type;

    };

    // Construct an option from a function type.

    template <typename Proto>
    struct option_prototype;

    template <typename R, typename... T>
    struct option_prototype <R(T...)> : option<T...> {};

  }

}

#endif

