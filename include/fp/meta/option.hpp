
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

    namespace impl {

      template <typename... T>
      struct option_case;

      template <typename T>
      struct option_case <T> {

        template <typename F, typename Z>
        struct elim_ : apply<F,T> {};

        template <typename F, typename Z>
        struct fold_ : apply<F,T> {};

      };

      template <>
      struct option_case <> {

        template <typename F, typename Z>
        struct elim_ : Z {};

        template <typename F, typename Z>
        struct fold_ : Z {};

      };

    }

    template <typename... T>
    struct option : impl::option_case<T...> {

      template <typename F>
      struct map_ : option<typename apply<F,T>::type...> {};

      template <typename F>
      struct bind_ : join<map<option,F>> {};

      typedef option type;

    };

    template <typename T>
    struct join <option<option<T>>> : option<T> {};

    template <>
    struct join <option<option<>>> : option<> {};

    template <>
    struct join <option<>> : option<> {};

    // Construct an option from a function type.

    template <typename Proto>
    struct option_prototype;

    template <typename R, typename... T>
    struct option_prototype <R(T...)> : option<T...> {};

    // Use option for SFINAE.

    template <typename Opt>
    struct option_sfinae : elim<Opt,fun<id>,empty> {};

  }

}

#endif

