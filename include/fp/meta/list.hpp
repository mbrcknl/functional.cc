
// Copyright (c) Matthew Brecknell 2012.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or a copy at http://www.boost.org/LICENSE_1_0.txt).

#ifndef FUNCTIONAL_CPP_INCLUDED_FP_META_VARIADIC_HPP
#define FUNCTIONAL_CPP_INCLUDED_FP_META_VARIADIC_HPP

#include "generic.hpp"

namespace fp {

  namespace meta {

    // A list of types.

    template <typename... Args>
    struct list;

    namespace impl {

      template <typename... Args>
      struct list_case;

      template <typename Arg, typename... Args>
      struct list_case <Arg,Args...> {

        template <typename F, typename Z>
        struct elim_ : apply<F,Arg,list<Args...>> {};

        template <typename F, typename Z>
        struct fold_ : apply<F,Arg,fold<list<Args...>,F,Z>> {};

      };

      template <>
      struct list_case <> {

        template <typename F, typename Z>
        struct elim_ : Z {};

        template <typename F, typename Z>
        struct fold_ : Z {};

      };

    }

    template <typename... Args>
    struct list : impl::list_case<Args...> {

      template <typename F>
      struct unpack_ : apply<F,Args...> {};

      template <typename T>
      struct cons_ : list<T,Args...> {};

      typedef list type;

    };

    template <typename F, typename List>
    struct unpack : List::type::template unpack_<F> {};

    template <typename T, typename List>
    struct cons : List::type::template cons_<T> {};

    template <typename XS, typename YS>
    struct concat : fold<XS,fun<cons>,YS> {};

  }

}

#endif
