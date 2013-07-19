
// Copyright (c) Matthew Brecknell 2013.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or a copy at http://www.boost.org/LICENSE_1_0.txt).

#ifndef FUNCTIONAL_CC_INCLUDED_FP_LAZY_HPP
#define FUNCTIONAL_CC_INCLUDED_FP_LAZY_HPP

#include <functional>
#include <memory>
#include <utility>

template <typename T>
class lazy {

  typedef std::function<T()> eval_type;

  struct variant {

    bool evaluated;

    union {
      eval_type * eval;
      T value;
    };

    variant(const eval_type & eval)
      : eval(new eval_type(eval))
      , evaluated(false)
      {}

    variant(eval_type && eval)
      : eval(new eval_type(std::move(eval)))
      , evaluated(false)
      {}

    T& get() {
      if (!evaluated) {
        eval_type * saved_eval = eval;
        try { new (&value) T((*eval)()); }
        catch { eval = saved_eval; throw; }
        delete saved_eval;
        evaluated = true;
      }
      return value;
    }

    ~variant() {
      if (evaluated) value.~T();
      else delete eval;
    }
  };

  std::shared_ptr<variant> variant_ptr;

public:

  template <typename Eval>
  lazy(const Eval & eval)
    : variant_ptr(std::make_shared<variant>(eval)) {}

  template <typename Eval>
  lazy(Eval && eval)
    : variant_ptr(std::make_shared<variant>(std::move(eval))) {}

  lazy() = delete;
  lazy(const lazy &) = default;
  lazy(lazy &&) = default;

  lazy & operator=(const lazy &) = default;
  lazy & operator=(lazy &&) = default;

  T& operator()() {
    return variant_ptr->get();
  }

  const T& operator()() const {
    return variant_ptr->get();
  }

};

#endif

