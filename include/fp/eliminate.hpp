
// Copyright (c) Matthew Brecknell 2013.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or a copy at http://www.boost.org/LICENSE_1_0.txt).

#ifndef FUNCTIONAL_CC_INCLUDED_FP_ELIMINATE_HPP
#define FUNCTIONAL_CC_INCLUDED_FP_ELIMINATE_HPP

#include <type_traits>
#include <utility>

#include <fp/call_traits.hpp>
#include <fp/meta/list.hpp>
#include <fp/meta/option.hpp>

namespace fp {

  struct _ {};

  template <typename... Specs>
  struct eliminate;

  namespace impl {

    // Check that each Specs... is of the form T(Args...).

    template <typename... Specs>
    struct check_eliminate_args;

    template <typename Spec, typename... Specs>
    struct check_eliminate_args <Spec,Specs...>
      : std::integral_constant<
          bool,
          std::is_function<Spec>::value &&
          check_eliminate_args<Specs...>::value
        > {};

    template <>
    struct check_eliminate_args <> : std::true_type {};

    // Match a function type (Func) against lists of argument types (SpecsList).
    // Return a tuple of:
    // - result types for matching argument type-lists,
    // - the matching argument type-lists,
    // - the unmatched argument type-lists.

    template <typename ResultTypes, typename MatchedSpecs, typename UnmatchedSpecs>
    struct match_result {

      typedef typename ResultTypes::type result_types;

      typedef typename MatchedSpecs::type matched_specs;
      typedef typename UnmatchedSpecs::type unmatched_specs;

      typedef match_result type;

    };

    template <typename Func, typename SpecsList>
    struct match_func_to_specs {

      struct nojoker;

      template <typename Jok, typename... Args>
      struct tag_res_spec {

        template <typename T>
        struct _apply_ : meta::list<T,Jok(Args...)> {};

        typedef tag_res_spec type;

      };

      template <typename Spec>
      struct result_spec;

      template <typename Ignore, typename... Args>
      struct result_spec <Ignore(Args...)>
        : meta::elim<
            result_of<Func(Args...)>, tag_res_spec<nojoker,Args...>,
            meta::elim<result_of<Func(_)>, tag_res_spec<_,Args...>, meta::list<>>
          > {};

      template <typename Spec, typename Match>
      struct step_impl {

        typedef typename Match::type::result_types result_types;
        typedef typename Match::type::matched_specs matched_specs;
        typedef typename Match::type::unmatched_specs unmatched_specs;

        struct unpack_match {

          template <typename...>
          struct _apply_
            : match_result<
                result_types, matched_specs, meta::cons<Spec,unmatched_specs>
              > {};

          template <typename R, typename S>
          struct _apply_ <R,S>
            : match_result<
                meta::cons<R,result_types>, meta::cons<S,matched_specs>,
                unmatched_specs
              > {};

          typedef unpack_match type;

        };

        typedef typename meta::unpack<
          unpack_match, meta::apply<meta::fun<result_spec>,Spec>
          >::type type;

      };

      template <typename Spec, typename Match>
      struct step : step_impl<Spec,Match>::type {};

      typedef typename meta::fold<SpecsList, meta::fun<step>,
        match_result<meta::list<>,meta::list<>,meta::list<>>
        >::type type;

    };

    // Calculate the result type for an elimination.
    // Return a meta::option<...> to indicate whether the elimination exists.

    template <typename FuncsList, typename SpecsList, typename Seed>
    struct eliminate_result;

    template <typename Func, typename FuncsList, typename SpecsList, typename Seed>
    struct elim_res_impl {

      typedef typename match_func_to_specs<Func,SpecsList>::type match;

      typedef typename match::result_types result_types;
      typedef typename match::unmatched_specs unmatched_specs;

      template <typename T, typename R>
      struct if_non_empty_impl {

        typedef meta::cons<T,R> TS;

        template <typename S>
        struct seed : meta::cons<S,TS> {};

        typedef meta::elim<Seed,meta::fun<seed>,TS> type_list;
        typedef fp::common_type_list<type_list> common_type;

        template <typename S>
        struct recurse
          : eliminate_result<FuncsList, unmatched_specs, meta::option<S>> {};

        typedef meta::bind<common_type,meta::fun<recurse>> type;

      };

      template <typename T, typename R>
      struct if_non_empty : if_non_empty_impl<T,R>::type {};

      typedef typename meta::elim<
        result_types, meta::fun<if_non_empty>, meta::option<>
        >::type type;

    };

    template <typename Func, typename... Funcs, typename SpecsList, typename Seed>
    struct eliminate_result <meta::list<Func,Funcs...>, SpecsList, Seed>
     : elim_res_impl<Func, meta::list<Funcs...>, SpecsList, Seed>::type {};

    template <typename Spec, typename... Specs, typename Seed>
    struct eliminate_result <meta::list<>, meta::list<Spec,Specs...>, Seed>
      : meta::option<> {};

    template <typename Seed>
    struct eliminate_result <meta::list<>, meta::list<>, Seed>
      : Seed {};

    // Build the elimination function as a set of overloads.

    template <typename Ret, typename Func, typename SpecsList>
    struct elim_overload;

    template <typename Top>
    struct overload_return;

    template <typename Ret, typename Func, typename SpecsList>
    struct overload_return <elim_overload<Ret,Func,SpecsList>> {
      typedef Ret type;
    };

    template <typename Top, typename SpecsList>
    struct overload_impl;

    template <typename Top, typename Jok, typename... Args, typename... Specs>
    struct overload_impl <Top,meta::list<Jok(Args...),Specs...>> {

      template <typename Joker, typename Dummy = void>
      struct overload {
        typename overload_return<Top>::type operator()(Args &&... args) const {
          return
            (static_cast<const Top *>(this)->func)
            (std::forward<Args>(args)...);
        }
      };

      template <typename Dummy>
      struct overload <_, Dummy> {
        typename overload_return<Top>::type operator()(Args &&... args) const {
          return (static_cast<const Top *>(this)->func)(_());
        }
      };

      // Multiple inheritance ensures no overloads are hidden.
      struct type : overload<Jok>, overload_impl<Top,meta::list<Specs...>>::type {};

    };

    template <typename Top>
    struct overload_impl <Top,meta::list<>> {
      struct type {};
    };

    template <typename Ret, typename Func, typename SpecsList>
    struct elim_overload
      : overload_impl<elim_overload<Ret,Func,SpecsList>,SpecsList>::type
    {
      elim_overload(Func && func) : func(std::forward<Func>(func)) {}
      Func && func;
    };

    template <typename Ret, typename FuncsList, typename SpecsList>
    struct eliminate_with;

    template <typename Ret, typename Func, typename... Funcs, typename SpecsList>
    struct eliminate_with <Ret, meta::list<Func,Funcs...>, SpecsList> {

      typedef typename match_func_to_specs<Func,SpecsList>::type match;

      typedef typename match::matched_specs matched_specs;
      typedef typename match::unmatched_specs unmatched_specs;

      typedef elim_overload<Ret,Func,matched_specs> overload;

      typedef typename eliminate_with<
        Ret, meta::list<Funcs...>, unmatched_specs
        >::type recurse;

      struct type : overload, recurse {
        type(Func && func, Funcs &&... funcs)
          : overload(std::forward<Func>(func))
          , recurse(std::forward<Funcs>(funcs)...) {}
      };

    };

    template <typename Ret>
    struct eliminate_with <Ret, meta::list<>, meta::list<>> {
      struct type {};
    };

    template <typename Ret>
    struct seed_result : meta::option<Ret> {};

    template <>
    struct seed_result <_> : meta::option<> {};

  }

  template <typename... Specs>
  struct eliminate {

    // Check that each Specs... is of the form T(Args...).

    static_assert(
      impl::check_eliminate_args<Specs...>::value,
      "fp::eliminate<...> template parameters must each be of the form T(Args...)"
    );

    template <typename Ret, typename... Funcs>
    struct result_type
      : meta::option_sfinae<impl::eliminate_result<
          meta::list<Funcs...>, meta::list<Specs...>,
          typename impl::seed_result<Ret>::type
        >> {};

    template <typename Ret, typename... Funcs>
    using eliminate_with = typename impl::eliminate_with<
      typename result_type<Ret,Funcs...>::type,
      meta::list<Funcs...>, meta::list<Specs...>
    >::type;

    template <typename Ret, typename... Funcs>
    static eliminate_with<Ret,Funcs...> with(Funcs &&... funcs) {
      return eliminate_with<Ret,Funcs...>(std::forward<Funcs>(funcs)...);
    }

  };

  template <typename Eliminate, typename Ret, typename... Funcs>
  struct eliminate_result : Eliminate::template result_type<Ret,Funcs...> {};

}

#endif

