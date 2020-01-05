//
// Created by phirasit on 12/28/19.
//
#include "grader/grader_pool.hpp"

#include <signal.h>
#include <stdlib.h>
#include <iostream>

static void keyboard_interrupt_handler(int sig);

static GraderPool *main_pool;

int main(int argc, char* argv[]) {
  if (argc <= 1) {
    std::cerr << "./usage [num-graders]" << std::endl;
    return 1;
  }
  
  const int num_graders = std::stoi(argv[1]);
  main_pool = new GraderPool(num_graders);
  
  signal(SIGINT, keyboard_interrupt_handler);
  main_pool->start();
  
  delete main_pool;
  
  return 0;
}

void keyboard_interrupt_handler(int sig) {
  main_pool->signal(GRADER_SIGNAL::KILL);
  exit(0);
}
