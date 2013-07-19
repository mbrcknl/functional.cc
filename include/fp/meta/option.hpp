
// Copyright (c) Matthew Brecknell 2013.
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
        struct _elim_ : apply<F,T> {};

        template <typename F, typename Z>
        struct _fold_ : apply<F,T> {};

      };

      template <>
      struct option_case <> {

        template <typename F, typename Z>
        struct _elim_ : Z {};

        template <typename F, typename Z>
        struct _fold_ : Z {};

      };

    }

    template <typename... T>
    struct option : impl::option_case<T...> {

      template <typename F>
      struct _map_ : option<typename apply<F,T>::type...> {};

      template <typename F>
      struct _bind_ : join<map<option,F>> {};

      typedef option type;

    };

    template <typename T>
    struct join_ <option<T>> : T {};

    template <>
    struct join_ <option<>> : option<> {};

    template <typename T, typename U>
    struct orelse_ <option<T>,U> : option<T> {};

    template <typename U>
    struct orelse_ <option<>,U> : U {};

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

