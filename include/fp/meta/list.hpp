
// Copyright (c) Matthew Brecknell 2013.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or a copy at http://www.boost.org/LICENSE_1_0.txt).

#ifndef FUNCTIONAL_CPP_INCLUDED_FP_META_LIST_HPP
#define FUNCTIONAL_CPP_INCLUDED_FP_META_LIST_HPP

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
        struct _elim_ : apply<F,Arg,list<Args...>> {};

        template <typename F, typename Z>
        struct _fold_ : apply<F,Arg,fold<list<Args...>,F,Z>> {};

      };

      template <>
      struct list_case <> {

        template <typename F, typename Z>
        struct _elim_ : Z {};

        template <typename F, typename Z>
        struct _fold_ : Z {};

      };

    }

    template <typename... Args>
    struct list : impl::list_case<Args...> {

      template <typename F>
      struct _unpack_ : apply<F,Args...> {};

      template <typename T>
      struct _cons_ : list<T,Args...> {};

      template <typename F>
      struct _map_ : list<typename apply<F,Args>::type...> {};

      template <typename F>
      struct _bind_ : join<map<list,F>> {};

      typedef list type;

    };

    template <typename F, typename List>
    struct unpack : List::type::template _unpack_<F> {};

    template <typename T, typename List>
    struct cons : List::type::template _cons_<T> {};

    template <typename XS, typename YS>
    struct concat : fold<XS,fun<cons>,YS> {};

    template <typename... T, typename... U>
    struct orelse_<list<T...>,list<U...>> : concat<list<T...>,list<U...>> {};

    template <typename... Args>
    struct join_ <list<Args...>> : fold<list<Args...>,fun<concat>,list<>> {};

    // Indexed access.

    template <std::size_t i, typename List>
    struct take;

    template <typename... Tail>
    struct take <0, list<Tail...>> : list<> {};

    template <std::size_t i, typename Head, typename... Tail>
    struct take <i, list<Head,Tail...>> : cons<Head,take<i-1,list<Tail...>>> {};

    template <std::size_t i, typename List>
    struct drop;

    template <typename... Tail>
    struct drop <0, list<Tail...>> : list<Tail...> {};

    template <std::size_t i, typename Head, typename... Tail>
    struct drop <i, list<Head,Tail...>> : drop<i-1,list<Tail...>> {};

    template <typename List>
    struct head;

    template <typename Head, typename... Tail>
    struct head <list<Head,Tail...>> : id<Head> {};

    template <typename List>
    struct tail : drop<1,List> {};

    template <std::size_t i, typename List>
    struct at : head<drop<i,List>> {};

    // Construct a list from a function type.

    template <typename Proto>
    struct list_prototype;

    template <typename R, typename... Args>
    struct list_prototype <R(Args...)> : list<Args...> {};

  }

}

#endif

