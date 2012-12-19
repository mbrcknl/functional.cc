
// Copyright (c) Matthew Brecknell 2012.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or a copy at http://www.boost.org/LICENSE_1_0.txt).

#include <iostream>
#include "../include/strict.hpp"

template <typename R> struct ret {};

template <typename T>
class tree {

  // data Tree t = Leaf | Node (Tree t) t (Tree t)

  struct leaf {};
  struct node;

  typedef strict::sum<leaf,node> tree_t;

  struct node {

    node(const tree_t & left, const T& val, const tree_t & right)
      : sub_left(left), value(val), sub_right(right) {}

    tree_t sub_left;
    tree_t sub_right;

    T value;

  };

  // The root of *this* tree (not excluding the possibility that
  // this tree might also be a subtree of one or more other trees).

  tree_t root;

  explicit tree(const tree_t & r) : root(r) {}

public:

  // Construct an empty tree.

  tree() : root(leaf()) {}

  // Construct a non-empty tree from a value and two subtrees.

  tree(const tree & left, const T & val, const tree & right)
    : root(node(left.root,val,right.root)) {}

  // Convenience: construct a singleton node.

  explicit tree(const T & val) : root(node(leaf(),val,leaf())) {}

  // Do something with this tree.

  template <typename R, typename Leaf, typename Node>
  R match(ret<R> r, Leaf && match_leaf, Node && match_node) const {
    return root.match <R> (
      [&](const leaf & e) { return match_leaf(); },
      [&](const node & n) { return match_node(tree(n.sub_left), n.value, tree(n.sub_right)); }
    );
  }

  tree(const tree &) = default;
  tree(tree &&) = default;

  tree & operator = (const tree &) = default;
  tree & operator = (tree &&) = default;

};

// Insert something into an ordered tree.
//
// insert :: Ord t => t -> Tree t -> Tree t
// insert x Leaf = Tree Leaf x Leaf
// insert x t@(Node left y right)
//   | x < y = Tree (insert x left) y right
//   | x > y = Tree left y (insert x right)
//   | otherwise = t

template <typename T>
tree<T> insert(const T& x, const tree<T> & tr) {

  typedef tree<T> tree_T;

  return tr.match /* <tree_T> */ (
    ret<tree_T>(),
    [&]() {
      return tree_T(tree_T(), x, tree_T());
    },
    [&](const tree_T & left, const T& y, const tree_T & right) {
      return x < y
           ? tree_T(insert(x, left), y, right)
           : x > y
           ? tree_T(left, y, insert(x, right))
           : tr;
    }
  );

}

// Map a function over values in tree.
//
// map_tree :: (t -> r) -> Tree t -> Tree r
// map_tree f Leaf = Leaf
// map_tree f (Node left x right)
//   = Node (map_tree f left) (f x) (map_tree f right)

template <typename R, typename T, typename F>
tree<R> map_tree(const F & f, const tree<T> & tr) {

  typedef tree<T> tree_T;
  typedef tree<R> tree_R;

  return tr.match /* <tree_R> */ (
    ret<tree_R>(),
    [&]() { return tree_R(); },
    [&](const tree_T & left, const T& x, const tree_T & right) {
      return tree_R(map_tree<R>(f, left), f(x), map_tree<R>(f, right));
    }
  );

}

// Display a tree.

template <typename T>
void display(std::ostream & out, const tree<T> & tr, std::string step, std::string indent) {

  typedef tree<T> tree_T;

  tr.match /* <void> */ (
    ret<void>(),
    [&]() {},
    [&](const tree_T & left, const T& x, const tree_T & right) {
      display(out, right, step, step + indent);
      out << indent << x << std::endl;
      display(out, left, step, step + indent);
    }
  );

}

template <typename T>
std::ostream & operator << (std::ostream & out, const tree<T> & tr) {
  display(out,tr,"  ","");
  return out;
}

