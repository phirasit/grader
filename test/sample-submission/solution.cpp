//
// Created by phirasit on 1/3/20.
//
#include <iostream>
#include <chrono>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
  int a, b;
  std::cin >> a >> b;
  
  // get time - should be fine
  auto start = std::chrono::system_clock::now();

  // this condition will make the solution wrong in some cases
  // this will not pass the second test of small group
  if ((a+b) == 100) {
    // this is invalid and should be killed by sandbox
    int _fd = open("/dev/null", O_RDONLY);
  }
  
  std::cout << (a+b) << std::endl;
  return 0;
}

