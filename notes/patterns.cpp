
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

}
