
void pattern_match_syntax() {

  data_type_1 data_value_1;
  data_type_2 data_value_2;

  // return_type_seed is optional if the common return type can be deduced
  // from the set of return types of the pattern clauses.

  auto result = match <return_type_seed> (data_value_1, data_value_2) (
    [&](pattern_type_1a x, pattern_type_2a y) { return expr1(x,y); },
    [&](pattern_type_1b x, pattern_type_2b y) { return expr2(x,y); }
  );

  // compile time: check that the patterns are exhaustive and non-overlapping.

}

void pattern_match_examples() {

  typedef fp::list<int> list1;
  typedef fp::list<list1> list2;

  list1 foo;
  list2 bar;

  using fp::nil;
  using fp::cons;

  auto r1 = match(foo) (
    [&](cnil<>) { return r1_1; },
    [&](cnil<int> p) { return r1_2(at<0>(p)); },
    [&](cons<int,int> p) {
      return r1_3(at<0>(p), at<1>(p), tail<2>(p));
    }
  );

  auto r2 = match(bar) (
    [&](cnil<>) { return r2_1; },
    [&](cnil<cnil<>>) { return r2_2; },
    [&](cnil<cnil<int>> p) { return r2_3(head(head(p))); },
    [&](cnil<cons<int,int>> p) { list1 &l = at<0>(p); return r2_4(at<0>(l), at<1>(l), tail<2>(l)); },
    [&](cons<list1,list1> p) { return r2_5(at<0>(p), at<1>(p), tail<2>(p)); }
  );

  // Alternative pattern syntax - might handle view patterns and guards better.
  // Also names things directly.

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

}
