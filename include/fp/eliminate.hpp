
// Copyright (c) Matthew Brecknell 2012.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or a copy at http://www.boost.org/LICENSE_1_0.txt).

#ifndef FUNCTIONAL_CPP_INCLUDED_FP_ELIMINATE_HPP
#define FUNCTIONAL_CPP_INCLUDED_FP_ELIMINATE_HPP

#include <type_traits>
#include <utility>

#include "call_traits.hpp"
#include "meta/list.hpp"
#include "meta/option.hpp"

namespace fp {

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

      template <typename Spec>
      struct result_of;

      template <typename Ret, typename... Args>
      struct result_of <Ret(Args...)> : fp::result_of<Func(Args...)> {};

      template <typename Spec, typename Match>
      struct step_impl {

        typedef typename Match::type::result_types result_types;
        typedef typename Match::type::matched_specs matched_specs;
        typedef typename Match::type::unmatched_specs unmatched_specs;

        typedef match_result<
          result_types, matched_specs, meta::cons<Spec,unmatched_specs>
          > if_no_match;

        template <typename T>
        struct if_match
          : match_result<
              meta::cons<T,result_types>, meta::cons<Spec,matched_specs>,
              unmatched_specs
            > {};

        typedef typename meta::elim<
          meta::apply<meta::fun<result_of>,Spec>, meta::fun<if_match>, if_no_match
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
        typedef fp::common_type<type_list> common_type;

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

    template <typename Top, typename SpecsList>
    struct overload_impl;

    template <typename Top, typename I, typename... Args, typename... Specs>
    struct overload_impl <Top,meta::list<I(Args...),Specs...>> {

      struct overload {

        typename Top::return_type
        operator()(Args &&... args) const {
          return
            (static_cast<const Top *>(this)->func)
            (std::forward<Args>(args)...);
        }

      };

      struct type : overload, overload_impl<Top,meta::list<Specs...>>::type {};

    };

    template <typename Top>
    struct overload_impl <Top,meta::list<>> {
      struct type {};
    };

    template <typename Ret, typename Func, typename SpecsList>
    struct elim_overload 
      : overload_impl<elim_overload<Ret,Func,SpecsList>,SpecsList>::type
    {
      typedef Ret return_type;
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

      typedef typename elim_overload<Ret,Func,matched_specs>::type overload;

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

  }

  template <typename... Specs>
  struct eliminate {

    // Check that each Specs... is of the form T(Args...).

    static_assert(
      impl::check_eliminate_args<Specs...>::value,
      "fp::eliminate<...> template parameters must each be of the form T(Args...)"
    );

    template <typename... Funcs>
    struct result_type
      : meta::option_sfinae<impl::eliminate_result<
          meta::list<Funcs...>, meta::list<Specs...>, meta::option<>
        >> {};

    template <typename... Funcs>
    using eliminate_with = typename impl::eliminate_with<
      typename result_type<Funcs...>::type,
      meta::list<Funcs...>, meta::list<Specs...>
    >::type;

    template <typename... Funcs>
    eliminate_with<Funcs...> with(Funcs &&... funcs) const {
      return eliminate_with<Funcs...>(std::forward<Funcs>(funcs)...);
    }

  };

  template <typename T> struct eliminate_result;

  template <typename Eliminate, typename... Funcs>
  struct eliminate_result <Eliminate(Funcs...)> 
    : Eliminate::template result_type<Funcs...> {};

}

#endif

