#include "grader_pool.hpp"

#include "file.hpp"
#include "grader.hpp"

#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

void* start_grader_thread(void* args);

static Grader* find_available_grader(std::vector<Grader>& graders) {
  for (Grader& grader : graders) {
    if (grader.get_status() == GRADER_STATUS::IDLE) {
      return &grader;
    }
  }
  return nullptr;
}

void GraderPool::assign_submission_to_grader() {
  Grader* grader = nullptr;
  while (
    (this->waiting_pool.size() > 0) &&
    (grader = find_available_grader(this->graders)) != nullptr) {
    // there are an ungraded submission and an available grader
    Submission* const submission = this->waiting_pool.get_first().value();
    grader->grade(submission);
  }
}

void GraderPool::add_submission(const Submission& submission) {
  auto* new_submission = new Submission(submission);
  this->waiting_pool.add(new_submission);
}

std::optional<Submission*> GraderPool::get_submission(const std::string& submission_id) const {
  return this->graded_pool.get(submission_id);
}

void GraderPool::start() {
  // start all graders
  for (int grader_id = 0; grader_id < this->get_num_graders(); ++grader_id) {
    Grader grader (grader_id);
    
    pthread_create(&grader.get_thread_id(), nullptr, &start_grader_thread, &grader);
    
    const std::string thread_name =
      std::string("grader-worker") + std::to_string(grader_id);
    pthread_setname_np(grader.get_thread_id(), thread_name.c_str());
    
    graders.emplace_back(grader);
  }
  
  // TODO start submission scheduler
  while (this->status != GRADER_POOL_STATUS::GRADER_POOL_TERMINATED) {
    this->assign_submission_to_grader();
    sleep(100);
  }
}

void GraderPool::signal(const GRADER_SIGNAL sig) {
  std::cerr << "keyboard interrupt" << std::endl;
  for (Grader& grader : graders) {
    grader.signal(sig);
  }
  switch (sig) {
    case GRADER_SIGNAL::KILL:
      this->status = GRADER_POOL_STATUS::GRADER_POOL_TERMINATED;
      break;
    default:
      break;
  }
}

void* start_grader_thread(void* args) {
  reinterpret_cast<Grader*>(args)
    ->start_grader();
  return nullptr;
}


