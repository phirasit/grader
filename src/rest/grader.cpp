//
// Created by phirasit on 12/28/19.
//
#include "logger.hpp"

#include "grader/grader_pool.hpp"
#include "router.hpp"

#include <signal.h>

static void keyboard_interrupt_handler(int sig);
static void* start_server(void*);
static GraderPool* main_pool;

int main(int argc, char* argv[]) {
  if (argc <= 2) {
    std::cerr << "usage: ./grader-server [num-graders] [port]" << std::endl;
    return 1;
  }
  
  // start server thread
  const std::string& port = std::string(argv[2]);
  const std::string& host = "*:" + port;
  
  pthread_t server_thread;
  pthread_create(&server_thread, nullptr, start_server, (void*) host.c_str());
  
  // start grader
  const int num_graders = std::stoi(argv[1]);
  main_pool = new GraderPool(num_graders);
  
  signal(SIGINT, keyboard_interrupt_handler);
  main_pool->start();
  
  return 0;
}

void keyboard_interrupt_handler(int sig) {
  main_pool->signal(GRADER_SIGNAL::KILL);
  exit(0);
}

void* start_server(void* args) {
  static const Logger logger("server");
  const std::string& host = std::string((char*) args);
  logger("starting a new server, listened on ", host);
  Pistache::Http::listenAndServe<GraderRestServer>(Address(host));
}
