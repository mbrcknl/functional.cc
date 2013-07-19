
#include <iostream>
#include <fp/unit.hpp>

int main() {
  std::cout << (unit() != tag<unit>()) << std::endl;
}

