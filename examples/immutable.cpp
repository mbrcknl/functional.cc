
#include <iostream>
#include <vector>

#include <fp/immutable.hpp>

int main() {
  std::cout << immutable<int>::value << std::endl;
  std::cout << immutable<const int>::value << std::endl;
  std::cout << immutable<std::vector<int>>::value << std::endl;
  std::cout << immutable<const std::vector<int>>::value << std::endl;
  std::cout << immutable<int[42]>::value << std::endl;
  std::cout << immutable<const int[]>::value << std::endl;
}

