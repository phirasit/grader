//
// Created by phirasit on 1/3/20.
//
#include <iostream>

int main() {
  int a, b;
  std::cin >> a >> b;
  
  // this condition will make the solution wrong in some cases
  // this will not pass the second test of small group
  if ((a+b) == 100) {
    ++b;
  }
  
  std::cout << (a+b) << std::endl;
  return 0;
}

