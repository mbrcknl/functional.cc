
// Example: eliminator usage.

class list {

  struct nil;
  struct node;

  sum<nil,node> root;

  // Each typename argument:
  //   cons_n(field_0,field_1,...)
  // defines one of the required/allowed parameter-lists
  // for an elimination.

  // The cons_n is ignored, and may be used as documentation.

  typedef eliminate<nil(),node(T,list)> elim_list;

public:

  // The eliminate_result template checks that Funs... exactly covers
  // the parameter-lists supplied to the eliminator template.
  // So does eliminator<...>::with(...).
  // If not, a static assertion error is produced.
  // The eliminate_result template calculates the common_type of the
  // result types of all Funs...

  template <typename... Funs>
  typename eliminate_result<elim_list(Funs...)>::type
  eliminate(Funs &&... funs) const {
    auto elim = elim_list::with(std::forward<Funs>(funs)...);

    return root.eliminate(
      [&](const nil &) { return elim(); },
      [&](const node & n) { return elim(n.head, list(n.tail)); }
    );
  }

};

my_result_type void example(const list<int> & mylist) {
  return mylist.eliminate(
    fp::returning<my_result_type>(),
    []() { return my_result_type(); },
    [](int x, const list<int> & xs) { return my_result_type(x,xs); }
  );
}

