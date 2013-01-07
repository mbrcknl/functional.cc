
#include <iostream>

// how to create a set of overloaded functions, meta-programmatically.

template <int i, typename... Args>
struct bar {
  void operator()(Args &&... args) {
    std::cout << i << "\n";
  }
};

template <int i, typename... Specs>
struct foo;

// important: multiple inheritance ensures that all overloads get
// the same priority, and therefore ambiguity can be detected.

template <int i, typename R, typename... Args, typename... Specs>
struct foo <i,R(Args...),Specs...> : bar<i,Args...>, foo<i+1,Specs...> {};

template <int i, typename R, typename... Args>
struct foo <i,R(Args...)> : bar<i,Args...> {};

int main() {
  foo<0,void(long),int(int,int)> f;
  f(42);
  f(43,44);
}

