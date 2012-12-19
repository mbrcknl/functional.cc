
#include <iostream>
#include "../include/unit.hpp"

int main() {
  std::cout << (unit() != tag<unit>()) << std::endl;
}

