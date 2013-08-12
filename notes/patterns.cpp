
using fp::_;
using fp::match;
using fp::with;
using fp::guarded;
using fp::view;
using fp::varg;
using fp::_1;
using fp::_2;

void pattern_match_syntax() {

  data_type_1 data_value_1;
  data_type_2 data_value_2;

  // fp::match is a variadic template function which takes zero or more data
  // values, and returns an object with a member template operator(), which
  // in turn takes zero or more match clauses.

  // The return_type_seed is optional if the common return type can be deduced
  // from the set of return types of the match clauses.

  // The basic form of a match clause is: fp::with<pattern...>(body).
  // There must be a one-to-one correspondence between data values passed to
  // fp::match and patterns passed to fp::with.

  // A pattern is a type. Often, a pattern is an instantiation of a class
  // template, where the template arguments are sub-patterns which are
  // matched against the corresponding sub-components of the matched data value.

  // The wildcard (fp::_) is a pattern which always matches its corresponding
  // data value or data value sub-component. Patterns are typically parameterised
  // by one or more wildcards to mark the positions where data value components
  // should be bound to variables in the match clause.

  // fp::with<...> is a variadic function template which takes a number of
  // patterns as explicit template arguments, followed by a single type deduced
  // from a single function argument, the match clause body. The body is a
  // non-overloaded callable object, typically a lambda, whose parameters must
  // have a one-to-one correspondence with the wildcards in the patterns.

  // When a pattern contains wildcards, the pattern itself will not carry enough
  // type information to check that the pattern match is well formed. The parameter
  // and return types of the body are also needed.

  // Therefore, fp::with must be a function template which returns an object of a
  // type constructed from the pattern and the body, and which can provide fp::match
  // with the information required to verify that the match is well formed.

  // Body parameter types should match the types of the components of the
  // respective data values at the positions of the respective wildcards, in the
  // case that the data value matches the pattern.
  // As an exception, a body parameter type may be the wildcard (fp::_),
  // in which case the value of the respective component of the data value
  // is ignored.

  // At compile time: check that the patterns are exhaustive and non-overlapping.

  auto result1 = match <return_type_seed> (data_value_1) (

    with <pattern_type_1a> // e.g. including 3 wildcards
      ([](binding_type_1a_x x, binding_type_1a_y y, binding_type_1a_z z) { return expr1(x,y,z); }),

    with <pattern_type_1b> // e.g. including 2 wildcards
      ([](binding_type_1b_x x, binding_type_1b_y y) { return expr2(x,y); })

  );

  // Multiple discriminants are implicitly tupled.

  auto result2 = match (data_value_1, data_value_2) (

    with <pattern_type_1a, pattern_type_2a> // e.g. including 3 wildcards.
      ([](binding_type_x x, binding_type_y y, binding_type_z z) { return expr1(x,y,z); }),

    with <pattern_type_1b, pattern_type_2b> // e.g. including 2 wildcards.
      ([](binding_type_x x, binding_type_y y) { return expr2(x,y); })

    // etc.

  );

}

void pattern_match_examples() {

  typedef fp::list<int> list1;
  typedef fp::list<list1> list2;

  list1 foo;
  list2 bar;

  using fp::nil;
  using fp::cons;

  // nil is a function.
  // cons is a function template with a leading parameter pack.
  // cons<H...,T,F> matches a list with head elements matching H...
  // and tail matching T (usually either nil or fp::_).
  // In cons<H...,T,F>, F is the deduced type of the clause body.
  // The function template returns an object of a type which carries
  // all the type information from H...,T,F.
  // The parameters of each lambda correspond to the wildcards in
  // the respective pattern.

  auto r5 = match(bar) (
    with <nil>
      ([]() { return r5_1; }),
    with <cons<nil,nil>>
      ([]() { return r5_2; }),
    with <cons<cons<_,nil>,nil>>
      ([](int x) { return r5_3(x); }),
    with <cons<cons<_,_,_>,nil>>
      ([](int x, int y, list1 zs) { return r5_4(x,y,zs); }),
    with <cons<_,_,_>>
      ([](list1 xs, list1 ys, list2 zs) { return r5_5(xs,ys,zs); })
  );

  // Pattern guards.
  // fp::guarded takes the place of fp::with, but expects a body that
  // returns a fp::option result. A guarded pattern only matches when
  // the body returns a non-none result.

  auto r6 = match<overlapping>(foo) (
    guarded <cons<_,nil>>
      ([](int x) { return maybe_r6_1(x); }),
    with <_>
      ([](list1 xs) { return r6_2(xs); })
  );

  // View patterns.
  // fp::view takes the place of fp::with, and is also a function template.
  // fp::view expects one or more view functions, and one or more
  // patterns (which may, in turn, be either fp::with or fp::view).

  auto r7 = match(foo) (

    // _1, _2 are the components to be passed to the view function.
    // _n is shorthand for varg<n,0>.
    // varg<n,f> specifies that the component should be passed as
    // argument n of view function f (both integers).
    // Thus, multiple view functions are allowed, but shorthand exists
    // for the common case where there is only one view function.

    view <cons<_1,_2,_>> (

      // View function.
      // This example only includes one view function, but there may be
      // several if the pattern includes varg<n,f> for different f.

      [](int i, int j) { return view7(i,j); },

      // Patterns which match the result of the view function first,
      // then any remaining components from the enclosing pattern.
      // If these patterns are complete and non-overlapping, then
      // the whole match may still be checked for the same properties.
      // If not, then the outer match must be marked as overlapping
      // or incomplete, as appropriate.

      with <view_pat_1,nil>
        ([](binding_type_x x) { return r7_1(x); }),

      with <view_pat_2,nil>
        ([](binding_type_y y) { return r7_2(y); }),

      with <_,cons<_,_>>
        ([](binding_type_z z, int k, list1 l) { return r7_3(z,k,l); })
    ),

    // No view arguments, so these are just ordinary patterns.

    with <cons<_,nil>>
      ([](int x) { return r7_4(x); }),

    with <nil>
      ([]() { return r7_5; })

  );

  // The guarded pattern above is equivalent to the following view pattern.

  auto r8 = match<overlapping>(foo) (

    view <cons<_1,nil>> (
      [](int x) { return maybe_r6_1(x); },
      with <just<_>> ([](result_t r) { return r; })
    ),

    with <_>
      ([](list1 xs) { return r6_2(xs); })

  );

}
