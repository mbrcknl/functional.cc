
// Copyright (c) Matthew Brecknell 2013.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or a copy at http://www.boost.org/LICENSE_1_0.txt).

#ifndef FUNCTIONAL_CC_INCLUDED_FP_META_GENERIC_HPP
#define FUNCTIONAL_CC_INCLUDED_FP_META_GENERIC_HPP

namespace fp {

  namespace meta {

    // Apply a type-level function: a struct with member template "type::apply_".

    template <typename F, typename... Args>
    struct apply_ : F::template _apply_<Args...> {};

    template <typename F, typename... Args>
    struct apply : apply_<typename F::type, Args...> {};

    // Wrap a template struct to make a type-level function.
    // Assumes the template struct gives its result via a member typedef.

    template <template <typename...> class F>
    struct fun {

      template <typename... Args>
      struct _apply_ : F<Args...> {};

      typedef fun type;

    };

    // Overloaded operations on type-level structures.
    // These (with postfix underscores) may be specialised to meta-types.

    template <typename T, typename... Destr>
    struct elim_ : T::template _elim_<Destr...> {};

    template <typename T, typename... Fold>
    struct fold_ : T::template _fold_<Fold...> {};

    template <typename T, typename F>
    struct map_ : T::template _map_<F> {};

    template <typename T>
    struct join_ : T::_join_ {};

    template <typename T, typename F>
    struct bind_ : T::template _bind_<F> {};

    template <typename T, typename U>
    struct orelse_ : T::template _orelse_<U> {};

    // Overloaded operations on type-level structures.
    // These (without postfix underscores) should be used in client code,
    // as they perform a normalisation step.

    template <typename T, typename... Destr>
    struct elim : elim_<typename T::type, Destr...> {};

    template <typename T, typename... Fold>
    struct fold : fold_<typename T::type, Fold...> {};

    template <typename T, typename F>
    struct map : map_<typename T::type, F> {};

    template <typename T>
    struct join : join_<typename T::type> {};

    template <typename T, typename F>
    struct bind : bind_<typename T::type, F> {};

    template <typename T, typename U>
    struct orelse : orelse_<typename T::type, typename U::type> {};

    // Meta-Identity.

    template <typename T>
    struct id {

      template <typename F>
      struct _elim_ : apply<F,T> {};

      template <typename F>
      struct _fold_ : apply<F,T> {};

      template <typename F>
      struct _map_ : id<typename apply<F,T>::type> {};

      struct _join_ : T {};

      template <typename F>
      struct _bind_ : apply<F,T> {};

      template <typename U>
      struct _orelse_ : id<T> {};

      typedef T type;

    };

    // Simple tuple type.

    template <typename Fst, typename Snd>
    struct tup {

      typedef typename Fst::type fst;
      typedef typename Snd::type snd;

      typedef tup type;

    };

    // Empty type.

    struct empty {};

  }

}

#endif

