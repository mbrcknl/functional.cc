
using fp::_;
using fp::match;
using fp::guarded;
using fp::tup;
using fp::varg;
using fp::_1;
using fp::_2;

void pattern_match_syntax() {

  data_type_1 data_value_1;
  data_type_2 data_value_2;

  // fp::match is a template function which returns an object
  // with a member template operator().

  // The return_type_seed is optional if the common return type can be deduced
  // from the set of return types of the match clauses.

  // Patterns should be parameterised with one or more wildcards (fp::_)
  // to mark the components to which variables should be bound.
  // An object of the wildcard type (fp::_()) is also a valid pattern on its own.
  // The function template fp::tup<_> is an alternative to fp::_().

  // A pattern may be a function template or an object with a template
  // member operator(). Because it typically contains wildcards, a pattern
  // may not carry enough type information to facilitate matching.
  // Therefore, the function template or template member operator() must
  // return an object of a type which does carry enough type information.
  // This type must be inferred from the parameter and return types of the
  // callable object passed to the pattern template.

  // Binding types should match the types of the components of the respective
  // data types at the positions of the respective wildcards, in the case
  // that the data value matches the pattern.
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
  // A single-parameter tuple pattern matches the same as its argument.

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

  // Pattern guards.

  auto r6 = match<overlapping>(foo) (
    guarded<cons<_,nil>>
      ([](int x) { return maybe_r6_1(x); }),
    tup<_>
      ([](list1 xs) { return r6_2(xs); })
  );

  // View patterns.

  auto r7 = match(foo) (

    // _1, _2 are the components to be passed to the view function.
    // _n is shorthand for varg<n,0>.
    // varg<n,f> specifies that the component should be passed as
    // argument n of view function f (both integers).
    // Thus, multiple view functions are allowed, but shorthand exists
    // for the common case where there is only one view function.

    cons<_1,_2,_>(

      // View function.
      // This exaple only includes one view function,
      // but there may be several if varg<n,f> for different f.

      [](int i, int j) { return view7(i,j); },

      // Patterns which match the result of the view function first,
      // then any remaining components from the outer pattern.
      // If these patterns are complete and non-overlapping, then
      // the whole match may still be checked for the same properties.
      // If not, then the outer match must be marked as overlapping
      // or incomplete, as appropriate.

      tup<view_pat_1,nil>
        ([](binding_type_x x) { return r7_1(x); }),

      tup<view_pat_2,nil>
        ([](binding_type_y y) { return r7_2(y); }),

      tup<_,cons<_,_>>
        ([](binding_type_z z, int k, list1 l) { return r7_3(z,k,l); })
    ),

    // No view arguments, so these are just ordinary patterns.

    cons<_,nil>
      ([](int x) { return r7_4(x); }),

    nil
      ([]() { return r7_5; })

  );

  // The guarded pattern above is equivalent to the following view pattern.

  auto r8 = match<overlapping>(foo) (
    cons<_1,nil>(
      [](int x) { return maybe_r6_1(x); },
      just<_> ([](result_t r) { return r; })
    ),
    tup<_>
      ([](list1 xs) { return r6_2(xs); })
  );

}
