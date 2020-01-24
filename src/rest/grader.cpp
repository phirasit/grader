//
// Created by phirasit on 12/28/19.
//
#include "pistache/endpoint.h"

#include "grader/grader_pool.hpp"
#include "logger.hpp"

#include "router.hpp"

#include <signal.h>

static void keyboard_interrupt_handler(int sig);
static void start_server(const std::string& host);
static GraderPool* main_pool;
static GraderRestServer* server;

static const Logger logger("server");

int main(int argc, char* argv[]) {
  if (argc <= 2) {
    std::cerr << "usage: ./grader-server [num-graders] [port]" << std::endl;
    return 1;
  }
  
  // register signal handler
  signal(SIGINT, keyboard_interrupt_handler);
  
  // start grader
  const int num_graders = std::stoi(argv[1]);
  main_pool = new GraderPool(num_graders);
  
  // start rest server
  const std::string& port = std::string(argv[2]);
  const std::string& host = "*:" + port;
  start_server(host);
  
  // start main thread
  main_pool->start();
  
  return 0;
}

void keyboard_interrupt_handler(int sig) {
  main_pool->signal(GRADER_SIGNAL::KILL);
  logger("stopping the server");
  server->shutdown();
  logger("server is stopped");
  exit(0);
}

void start_server(const std::string& host) {
  
  // create a single thread
  auto opts = Http::Endpoint::options().threads(1);
  server = new GraderRestServer(Address(host), main_pool);
  
  server->init(opts);
  server->start();
  logger("starting a new server, listened on ", host);
}
