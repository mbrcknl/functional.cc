
// Copyright (c) Matthew Brecknell 2013.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or a copy at http://www.boost.org/LICENSE_1_0.txt).

#ifndef FUNCTIONAL_CC_INCLUDED_FP_LIST_HPP
#define FUNCTIONAL_CC_INCLUDED_FP_LIST_HPP

#include <functional>
#include "strict.hpp"

template <typename T>
class list {

  struct unit {};
  struct node;

  typedef strict::sum<unit,node> type;

  struct node {

    template <typename... Args>
    node(const type & xs, Args &&... args)
      : x(std::forward<Args>(args)...), xs(xs) {}

    template <typename... Args>
    node(type && xs, Args &&... args)
      : x(std::forward<Args>(args)...), xs(std::move(xs)) {}

    T x;
    type xs;

  };

  type value;

public:

  list()
    : value(unit()) {}

  template <typename Arg, typename... Args>
  list(const list & xs, Arg && arg, Args &&... args)
    : value(node(xs, std::forward<Arg>(arg), std::forward<Args>(args)...)) {}

  template <typename Arg, typename... Args>
  list(list && xs, Arg && arg, Args &&... args)
    : value(node(std::move(xs), std::forward<Arg>(arg), std::forward<Args>(args)...)) {}

  template <typename R, typename Unit, typename Node>
  R match(Unit && match_unit, Node && match_node) {
    return value.match <R> (
      [&](unit & u) { return std::forward<Unit>(match_unit)(); },
      [&](node & n) { return std::forward<Node>(match_node)(n.x, n.xs); }
    );
  }

private:

  template <typename Iter>
  static list iter(Iter begin, Iter end, std::forward_iterator_tag tag) {
    if (begin == end) return list();
    else {
      typename std::iterator_traits<Iter>::reference referred = *begin;
      return list(from_iterator(++begin, end, tag), referred);
    }
  }

  template <typename Iter>
  static list iter(Iter begin, Iter end, std::bidirectional_iterator_tag) {
    auto rend = std::reverse_iterator<Iter>(begin);
    list result;
    for (auto i = std::reverse_iterator<Iter>(end); i != rend; ++i)
      result = list(std::move(result), *i);
    return result;
  }

  template <typename Iter>
  typename std::iterator_traits<Iter>::iterator_category
  category() {
    return typename std::iterator_traits<Iter>::iterator_category();
  }

public:

  template <typename Iter>
  list(Iter begin, Iter end) : list(iter(begin, end, category<Iter>())) {}

  template <typename U>
  list(std::initializer_list<U> l) : list(l.begin(), l.end()) {}

};

template <typename T>
std::function<T()> ret(const T & x) {
  return [&x]() { return x; };
}

template <typename T>
std::function<T()> ret(T && x) {
  return [&x]() { return std::move(x); };
}

template <typename T, typename U>
bool operator == (const list<T> & xs, const list<U> & ys) {
  return xs.match(
    ret(ys.match(ret(true), [](U& y, const list<U> & yr) { return false; })),
    [&](T& x, const list<T> & xr) {
      return ys.match(
        ret(false),
        [&](U& y, const list<U> & yr) {
          return x == y && xr == yr;
        }
      );
    }
  );
}

template <typename T, typename U, typename C>
bool operator != (const list<T> & xs, const list<U> & ys) {
  return !(xs == ys);
}

template <typename T, typename U>
bool operator <= (const list<T> & xs, const list<U> & ys) {
  return xs.match(
    ret(ys.match(ret(true), [](U& y, const list<U> & yr) { return true; })),
    [&](T& x, const list<T> & xr) {
      return ys.match(
        ret(false),
        [&](U& y, const list<U> & yr) {
          return x <= y || (x == y && xr <= yr);
        }
      );
    }
  );
}

template <typename T, typename U, typename C>
bool operator >= (const list<T> & xs, const list<U> & ys) {
  return ys <= xs;
}

template <typename T, typename U>
bool operator < (const list<T> & xs, const list<U> & ys) {
  return xs.match(
    ret(ys.match(ret(false), [](U& y, const list<U> & yr) { return true; })),
    [&](T& x, const list<T> & xr) {
      return ys.match(
        ret(false), [&](U& y, const list<U> & yr) {
          return x < y || (x == y && xr < yr);
        }
      );
    }
  );
}

template <typename T, typename U>
bool operator > (const list<T> & xs, const list<U> & ys) {
  return ys < xs;
}

template <typename Base, typename Rec, typename T>
auto foldl(Rec && rec, Base && base, const list<T> & xs) -> decltype(base()) {
  return xs.match(
    base,
    [&](T& head, const list<T> & tail) {
      return foldl(rec, ret(rec(base(), head)), tail);
    }
  );
}

template <typename Base, typename Rec, typename T>
auto foldr(Rec rec, Base base, const list<T> & xs) -> decltype(base()) {
  return xs.match(
    base,
    [&](T& head, const list<T> & tail) {
      return rec(head, foldr(rec, base, tail));
    }
  );
}

template <typename T>
list<T> concat(const list<T> & xs, const list<T> & ys) {
  return foldr(
    [](T& x, list<T> && r) { return cons(x, std::move(r)); },
    ret(ys),
    xs
  );
}

template <typename T>
list<T> reverse(const list<T> & xs) {
  return foldl(
    [](list<T> && r, T& x) { return cons(x, std::move(r)); },
    ret(list<T>()),
    xs
  );
}

template <typename R, typename T, typename F>
list<R> map(const list<T> & xs, const F & f) {
  return foldr(
    [&f](T& x, list<R> && r) { return cons(f(x), std::move(r)); },
    ret(list<R>()),
    xs
  );
}

template <typename T, typename P>
list<T> filter(const list<T> & xs, const P & p) {
  return foldr(
    [&p](T& x, list<T> && r) {
      return p(x) ? cons(x, std::move(r)) : std::move(r);
    },
    ret(list<T>()),
    xs
  );
}

template <typename T, typename A>
void foreach(const list<T> & xs, A && a) {
  foldl(
    [&]() {},
    [&](list<T> && r, T& x) { a(x); }
  );
}

template <typename T>
std::ostream & operator << (std::ostream & out, const list<T> & xs) {
  out << '{';

  xs.match <void> (
    [&]() {},
    [&](T& head, const list<T> & tail) {
      out << head;
      foreach(
        tail,
        [&](const T& x) { out << ',' << x; }
      );
    }
  );

  return out << '}';
}

#endif

