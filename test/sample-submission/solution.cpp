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

  // these conditions will make the solution wrong in some cases
  // this will not pass the second test of small group
  if ((a+b) == 100) { // case 3
    // this is invalid and should be killed by sandbox
    // triggering SIGSEGV
    int _fd = open("/dev/null", O_RDONLY);
  }
  
  if ((a+b) == 115) { // case 4
    // this will make an infinite loop
    // sandbox will kill by triggering SIGXCPU
    while (true) {}
  }
  
  if ((a+b) == 101) { // case 5
    // this will trigger SIGSEGV
    int *arr = new int[1'000'000'000];
  }
  
  std::cout << (a+b) << std::endl;
  return 0;
}

