
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

    // Calculate the result type for an elimination.
    // Return a meta::option<...> to indicate whether the elimination exists.

    template <typename Func, typename SpecsList>
    struct elim_res_one {

      template <typename Spec>
      struct result_of;

      template <typename Ret, typename... Args>
      struct result_of <Ret(Args...)> : fp::result_of<Func(Args...)> {};

      template <typename Spec, typename Tup>
      struct step_impl {

        typedef typename Tup::type::fst arg_result_types;
        typedef typename Tup::type::snd arg_specs_list;

        typedef meta::tup<
          arg_result_types, meta::cons<Spec,arg_specs_list>
          > if_no_match;

        template <typename T>
        struct if_match
          : meta::tup<meta::cons<T,arg_result_types>, arg_specs_list> {};

        typedef meta::apply<meta::fun<result_of>,Spec> match;

        typedef typename meta::elim<
          match, meta::fun<if_match>, if_no_match
          >::type result_type;

      };

      template <typename Spec, typename Tup>
      struct step : step_impl<Spec,Tup>::result_type {};

      typedef typename meta::elim<
        SpecsList, meta::fun<step>, meta::tup<meta::list<>,meta::list<>>
        >::type result_type;

    };

    template <typename FuncsList, typename SpecsList, typename Seed>
    struct eliminate_result;

    template <typename Func, typename FuncsList, typename SpecsList, typename Seed>
    struct elim_res_impl {

      typedef typename elim_res_one<Func,SpecsList>::result_type one_result;

      typedef typename one_result::fst::type one_result_type_list;
      typedef typename one_result::snd::type one_result_specs_list;

      template <typename T, typename R>
      struct if_non_empty_impl {

        typedef meta::cons<T,R> TS;

        template <typename S>
        struct seed : meta::cons<S,TS> {};

        typedef meta::elim<Seed,meta::fun<seed>,TS> type_list;
        typedef fp::common_type<type_list> common_type;

        template <typename S> struct recurse
          : eliminate_result<
              typename FuncsList::type,
              one_result_specs_list,
              meta::option<S>
            > {};

        typedef meta::bind<common_type,meta::fun<recurse>> result_type;

      };

      template <typename T, typename R>
      struct if_non_empty : if_non_empty_impl<T,R>::result_type {};

      typedef typename meta::elim<
        one_result_type_list, meta::fun<if_non_empty>, meta::option<>
        >::type result_type;

    };

    template <typename Func, typename... Funcs, typename SpecsList, typename Seed>
    struct eliminate_result <meta::list<Func,Funcs...>, SpecsList, Seed>
     : elim_res_impl<Func, meta::list<Funcs...>, SpecsList, Seed>::result_type {};

    template <typename Spec, typename... Specs, typename Seed>
    struct eliminate_result <meta::list<>, meta::list<Spec,Specs...>, Seed>
      : meta::option<> {}; // unmatched Spec.

    template <typename Seed>
    struct eliminate_result <meta::list<>, meta::list<>, Seed>
      : Seed {};

    // Build the elimination function.

    template <typename Ret, typename FuncsList, typename SpecsList>
    struct eliminate_with;

    template <typename Ret, typename Func, typename FuncsList, typename SpecsList>
    struct elim_with_impl {

    };

    template <typename Ret, typename Func, typename... Funcs, typename SpecsList>
    struct eliminate_with <Ret, meta::list<Func,Funcs...>, SpecsList>
      : elim_with_impl<Ret, Func, meta::list<Funcs...>, SpecsList>::result_type
    {

      typedef typename elim_with_impl<
        Ret, Func, meta::list<Funcs...>, SpecsList
        >::result_type base_type;

      eliminate_with(Func && func, Funcs &&... funcs)
        : base_type(std::forward<Func>(func), std::forward<Funcs>(funcs)...) {}

    };

    template <typename Ret>
    struct eliminate_with <Ret, meta::list<>, meta::list<>> {};

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
      : meta::option_sfinae<
          impl::eliminate_result<
            meta::list<Funcs...>, meta::list<Specs...>, meta::option<>
          >
        > {};

    template <typename... Funcs>
    using eliminate_with = impl::eliminate_with<
      typename result_type<Funcs...>::type,
      meta::list<Funcs...>, meta::list<Specs...>
    >;

    template <typename... Funcs>
    eliminate_with<Funcs...> with(Funcs &&... funcs) {
      return eliminate_with<Funcs...>(std::forward<Funcs>(funcs)...);
    }

  };

  template <typename T> struct eliminate_result;

  template <typename Eliminate, typename... Funcs>
  struct eliminate_result <Eliminate(Funcs...)> 
    : Eliminate::template result_type<Funcs...> {};

}

#endif

