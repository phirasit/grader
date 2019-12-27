#include "file.hpp"
#include "grader/grader.hpp"

#include <pthread.h>
#include <signal.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/mount.h>
#include <unistd.h>

#include <iostream>
#include <vector>

static void keyboard_interrupt_handler(int arg);
static void signal_graders(GRADER_SIGNAL sig);
static int create_grader(const int id);

void* start_thread(void* args);

static std::vector<Grader> graders;

int main(int argc, char *argv[]) {
  if (argc <= 1) {
    std::cerr << "[usage] ./grader [num-graders]" << std::endl;
    return 1;
  }
  
  const long num_graders = std::strtol(argv[1], nullptr, 10);
  
  signal(SIGINT, keyboard_interrupt_handler);
  
  // start all graders
  for (int grader_id = 0; grader_id < num_graders; ++grader_id) {
    Grader grader (grader_id);
    
    pthread_create(&grader.get_thread_id(), nullptr, &start_thread, &grader);
    
    const std::string thread_name =
      std::string("grader-worker") + std::to_string(grader_id);
    pthread_setname_np(grader.get_thread_id(), thread_name.c_str());
    
    graders.emplace_back(grader);
  }
  
  // TODO start submission scheduler
  while (true) {
    sleep(100);
    break;
  }
}

void keyboard_interrupt_handler(int arg) {
  std::cerr << "keyboard interrupt" << std::endl;
  signal_graders(GRADER_SIGNAL::KILL);
  exit(0);
}

void signal_graders(GRADER_SIGNAL sig) {
  for (Grader& grader : graders) {
    grader.signal(sig);
  }
}

void* start_thread(void* args) {
  reinterpret_cast<Grader*>(args)
    ->start_grader();
  return nullptr;
}


