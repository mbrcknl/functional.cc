
#include <iostream>
#include <string>

#include "../include/option.hpp"

struct myint {
  int i;
  myint() : i() { std::cout << "myint() " << i << std::endl; }
  myint(int i) : i(i) { std::cout << "myint(int) " << i << std::endl; }
  myint(const myint & i) : i(i.i) { std::cout << "myint(const myint &) " << i.i << std::endl; }
  myint(myint && i) : i(i.i) { std::cout << "myint(myint &&) " << i.i << std::endl; }
  ~myint() { std::cout << "~myint() " << i << std::endl; }
};

option<std::string> fun(const myint & i) {
  std::cout << "fun(" << i.i << ")" << std::endl;
  return i.i >= 42 ? std::to_string(i.i) : none<std::string>();
}

int main() {
  option<myint> foo;
  option<myint> bar = some<myint>();
  option<myint> baz = myint(42);

  for (const option<myint> & i: {foo,bar,baz}) {
    (i >> fun)(
      [](const std::string & s) { std::cout << "something! " << s << std::endl; },
      []() { std::cout << "nothing!" << std::endl; }
    );
  }
}

