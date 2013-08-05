
using fp::_;
using fp::match;
using fp::guarded;
using fp::view;

void pattern_match_syntax() {

  data_type_1 data_value_1;
  data_type_2 data_value_2;

  // fp::match is a template function which returns an object
  // with a member template operator().

  // The return_type_seed is optional if the common return type can be deduced
  // from the set of return types of the pattern clauses.

  // Pattern types should be parameterised with one or more wildcards (fp::_)
  // to mark the components to which variables should be bound.
  // The wildcard (fp::_) is also a valid pattern type on its own.

  // Binding types should match the types of the components of the respective
  // data types at the positions of the respective wildcards, in the case
  // that the data value actually matches the pattern type.
  // As an exception, the binding type may be the wildcard (fp::_),
  // in which case the value of the respective component of the data value
  // is ignored.

  // At compile time: check that the patterns are exhaustive and non-overlapping.

  auto result1 = match <return_type_seed> (data_value_1) (

    pattern_type_1a // e.g. including 3 wildcards
      ([](binding_type_1a_x x, binding_type_1a_y y, binding_type_1a_z z) { return expr1(x,y,z); }),

    pattern_type_1b // e.g. including 2 wildcards
      ([](binding_type_1b_x x, binding_type_1b_y y) { return expr2(x,y); })

  );

  // Multiple discriminants are implicitly tupled.
  // Pattern transformers (fp::guarded, fp::view) implicitly unpack tuples
  // if given multiple arguments.

  auto result2 = match (data_value_1, data_value_2) (

    tup<pattern_type_1a,pattern_type_2a>
      ([](binding_type_x x, binding_type_y y, binding_type_z z) { return expr1(x,y,z); }),

    tup<pattern_type_1b,pattern_type_2b>
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

  auto r3 = match(bar) (
    cnil<>
      ([]() { return r3_1; }),
    cnil<cnil<>>
      ([]() { return r3_2; }),
    cnil<cnil<int>>
      ([](int x) { return r3_3(x); }),
    cnil<cons<int,int,list1>>
      ([](int x, int y, list1 zs) { return r3_4(x,y,zs); }),
    cons<list1,list1,list2>
      ([](list1 xs, list1 ys, list2 zs) { return r3_5(xs,ys,zs); })
  );

  // If patterns are template functions, maybe we can infer the types
  // of the components of the patterns from the types of the arguments to the
  // lambda. Here, the lambda binds exactly one variable to each wildcard
  // in the pattern.

  auto r4 = match(bar) (
    cnil<>
      ([]() { return r4_1; }),
    cnil<cnil<>>
      ([]() { return r4_2; }),
    cnil<cnil<_>>
      ([](int x) { return r4_3(x); }),
    cnil<cons<_,_,_>>
      ([](int x, int y, list1 zs) { return r4_4(x,y,zs); }),
    cons<_,_,_>
      ([](list1 xs, list1 ys, list2 zs) { return r4_5(xs,ys,zs); })
  );

  // Here, cnil<t...> is now equivalent to cons<t...,nil>, so the above is the
  // same as the following.

  auto r5 = match(bar) (
    nil
      ([]() { return r5_1; }),
    cons<nil,nil>
      ([]() { return r5_2; }),
    cons<cons<_,nil>,nil>
      ([](int x) { return r5_3(x); }),
    cons<cons<_,_,_>,nil>
      ([](int x, int y, list1 zs) { return r5_4(x,y,zs); }),
    cons<_,_,_>
      ([](list1 xs, list1 ys, list2 zs) { return r5_5(xs,ys,zs); })
  );

  // Pattern guards. Can these be made more general?

  auto r6 = match(foo) (
    guarded<cons<_,nil>>
      ([](int x) { return maybe_r6_1(x); }),
    _
      ([](list1 xs) { return r6_2(xs); })
  );

  // View patterns. I'm not really convinced by this yet.

  auto r7 = match(foo,bar) (
    view(
      [](const list1& foo_, const list2& bar_) { return expr7(foo_,bar_); },
      pattern_from_view_1([](pat1_1 x, pat1_2 y, pat1_3 z) { return r7_1(x,y,z); })
      pattern_from_view_2([](pat2_1 x, pat2_2 y) { return r7_2(x,y); })
    )
  );

}
