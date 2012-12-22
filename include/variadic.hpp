
// Copyright (c) Matthew Brecknell 2012.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or a copy at http://www.boost.org/LICENSE_1_0.txt).

#ifndef MASALA_INCLUDED_VARIADIC_HPP
#define MASALA_INCLUDED_VARIADIC_HPP

namespace fp {

  template <typename F, typename... Args>
  struct apply : F::template apply<Args...> {};

  template <typename F, typename Pack>
  struct unpack : Pack::template unpack<F> {};

  // Directly construct a list of types.

  template <typename... Args>
  struct pack {

    template <typename F>
    struct unpack : fp::apply<F,Args...> {};

  };

  // Iteratively build a list of types.

  template <typename T, typename Pack>
  class cons {

    template <typename F>
    struct prep {

      template <typename... Args>
      struct apply : fp::apply<F,T,Args...> {};

    };

  public:

    template <typename F>
    struct unpack : fp::unpack<prep<F>,Pack> {};

  };

  // Destruct a list of types.

  

}

#endif // MASALA_INCLUDED_VARIADIC_HPP
