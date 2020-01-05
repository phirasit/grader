#include "grader/grader_pool.hpp"

#include "file.hpp"
#include "grader/grader.hpp"
#include "logger.hpp"

#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

void* start_grader_thread(void* args);

void GraderPool::assign_submission_to_grader() {
  for (Grader* const grader : this->graders) {
    if (grader->get_status() == GRADER_STATUS_DONE) {
      this->graded_pool.add(grader->get_submission());
      grader->grade(nullptr);
    }
    
    if (this->get_waiting_pool_size() > 0 && grader->get_status() == GRADER_STATUS_IDLE) {
      Submission* const submission = this->waiting_pool.get_first().value();
      grader->grade(submission);
    }
  }
}

int GraderPool::add_submission(const Submission& submission) {
  if (this->get_status() == GRADER_POOL_STATUS::GRADER_POOL_RUNNING) {
    this->logger("receive new submission: ", submission.get_submission_id());
    auto *new_submission = new Submission(submission);
    this->waiting_pool.add(new_submission);
    return 0;
  }
  this->logger("[warning] ignore submission: ", submission.get_submission_id());
  return 1;
}

std::optional<Submission*> GraderPool::get_submission(const std::string& submission_id) const {
  return this->graded_pool.get(submission_id);
}

void GraderPool::start() {
  this->status = GRADER_POOL_STATUS::GRADER_POOL_INIT;
  this->logger("starting grader pool...");
  
  // start all graders
  for (int grader_id = 0; grader_id < this->get_num_graders(); ++grader_id) {
    auto* grader = new Grader(grader_id);
    
    pthread_create(&grader->get_thread_id(), nullptr, &start_grader_thread, (void*) grader);
    
    const std::string thread_name =
      std::string("grader-worker") + std::to_string(grader_id);
    pthread_setname_np(grader->get_thread_id(), thread_name.c_str());
    
    this->graders.emplace_back(grader);
  }
  
  this->status = GRADER_POOL_STATUS::GRADER_POOL_RUNNING;
  this->logger("grader pool is ready for submissions");
  
  // TODO start submission scheduler
  while (this->status != GRADER_POOL_STATUS::GRADER_POOL_TERMINATED) {
    // if all workers are terminated
    if (std::all_of(this->graders.begin(), this->graders.end(), [] (const Grader* grader) {
      return grader->is_terminated();
    })) {
      this->logger("all workers are terminated");
      break;
    }
    
    // assign submission to grader
    this->assign_submission_to_grader();
    
    usleep(1000);
  }
}

void GraderPool::stop() {
  this->logger("stopping graders...");
  this->signal(GRADER_SIGNAL::KILL);
  this->logger("grader pool stopped");
}

void GraderPool::signal(const GRADER_SIGNAL sig) {
  this->logger("receive signal: ", sig);
  for (Grader* grader : this->graders) {
    this->logger("signal worker", grader->get_grader_id());
    grader->signal(sig);
  }
  switch (sig) {
    case GRADER_SIGNAL::KILL:
      for (Grader* grader : this->graders) {
        pthread_join(grader->get_thread_id(), nullptr);
      }
      this->status = GRADER_POOL_STATUS::GRADER_POOL_TERMINATED;
      break;
    default:
      break;
  }
}

void* start_grader_thread(void* args) {
  return reinterpret_cast<void*>
    (reinterpret_cast<Grader*>(args)
      ->start_grader());
}


