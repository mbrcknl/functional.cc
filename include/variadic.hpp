
// Copyright (c) Matthew Brecknell 2012.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or a copy at http://www.boost.org/LICENSE_1_0.txt).

#ifndef MASALA_INCLUDED_VARIADIC_HPP
#define MASALA_INCLUDED_VARIADIC_HPP

namespace fp {

  template <typename... Args>
  struct pack {

    template <template <typename...> class F>
    struct apply : F<Args...> {};

  };

  template <typename T, typename Pack>
  class cons {

    template <template <typename...> class F>
    struct apply_impl {

      template <typename... Args>
      struct cons_t : F<T,Args...> {};

    };

  public:

    template <template <typename...> class F>
    struct apply : Pack::template apply<apply_impl<F>::template cons_t> {};

  };

}

#endif // MASALA_INCLUDED_VARIADIC_HPP
