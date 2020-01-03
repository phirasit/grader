//
// Created by phirasit on 1/2/20.
//
#include "catch.hpp"
#include "../src/grader/grade_script.cpp"
#include "../src/grader/grader.cpp"
#include "../src/grader/grader_pool.cpp"
#include "../src/grader/submission_pool.cpp"
#include "yaml-cpp/yaml.h"

#include "submission.hpp"

#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

static void* start_grader_pool_thread(void* args) {
  reinterpret_cast<GraderPool*> (args)
    ->start();
  return nullptr;
}

TEST_CASE("grader pool", "[grader-pool][end-to-end]") {
  GraderPool grader_pool (1);
  const Submission& submission = load_sample_submission();
  
  SECTION("end-to-end test") {
    // start grader
    REQUIRE(grader_pool.get_status() == GRADER_POOL_INIT);
    pthread_t grader_thread;
    pthread_create(&grader_thread, nullptr, &start_grader_pool_thread, (void *) &grader_pool);
    
    while (grader_pool.get_status() != GRADER_POOL_RUNNING) usleep(1000);
    REQUIRE(grader_pool.get_status() == GRADER_POOL_RUNNING);
    
    // add a submission
    REQUIRE(grader_pool.add_submission(submission) == 0);
    while (grader_pool.get_waiting_pool_size() > 0) usleep(1000);
    
    // stop grader
    grader_pool.stop();
    REQUIRE(grader_pool.get_status() == GRADER_POOL_TERMINATED);
  }
  
  SECTION("submit without grader starting") {
    // REQUIRE(grader_pool.add_submission(submission) > 0);
  }
}
