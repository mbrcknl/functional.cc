
// Illustration of ADT usage.

template <typename t> struct list;

// Define constructors.

// Here, 'tup' stands for 'tuple'?
struct nil : tup<nil()> {
  // Inherit constructor.
  using tup<nil()>::tup;
};

template <typename T>
struct cons : tup<cons(T,list<T>)> {
  using tup<cons(T,list<T>)>::tup;
};

// Define the Abstract Data Type, using constructors.

template <typename T>
struct list : adt<list(nil,cons)> {
  using adt<list(nil,cons)>::adt;
};

// Perhaps add optional parameters to adt<...> to switch on auto-deriving of
// Eq, Ord, Show, Enum, Bounded, etc.
//
// Can we support user-defined auto-deriving?

