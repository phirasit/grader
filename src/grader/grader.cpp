#include "file.hpp"
#include "grader.hpp"
#include "task.hpp"

#include <sys/mount.h>
#include <unistd.h>

#include <iostream>

void Grader::grade(Submission* submission) {
  if (this->get_status() == GRADER_STATUS::GRADER_STATUS_IDLE) {
    this->submission = submission;
    this->logger("new submission: ", submission->get_submission_id());
    this->status = GRADER_STATUS::GRADER_STATUS_NEW;
  }
}

static int init_grader(const int id) {
  file::create_folder(file::get_path("/grader/") + std::to_string(id));
  file::create_folder(file::get_path("/run/") + std::to_string(id));
  return 0;
}

static int cleanup_grader(const int id) {
  file::remove_folder(file::get_path("/run/") + std::to_string(id));
  file::remove_folder(file::get_path("/grader/") + std::to_string(id));
  return 0;
}

static void terminate(Grader* grader) {
  while (grader->get_status() == GRADER_STATUS_BUSY) {
    usleep(1000);
  }
  
  if (grader->get_status() != GRADER_STATUS_INIT) {
    cleanup_grader(grader->get_grader_id());
  }
}

void Grader::signal(GRADER_SIGNAL sig) {
  this->logger("receive signal: ", sig);
  switch (sig) {
    case GRADER_SIGNAL::KILL:
      this->logger("terminating...");
      terminate(this);
      this->status = GRADER_STATUS::GRADER_STATUS_TERMINATED;
      this->logger("terminated");
      break;
    default:
      break;
  }
}

int Grader::start_grader() {
  this->logger("starting grader...");
  
  this->status = GRADER_STATUS::GRADER_STATUS_INIT;
  if (init_grader(this->get_grader_id())) {
    this->logger("grader is failed to start");
    this->status = GRADER_STATUS::GRADER_STATUS_ERROR;
    this->logger("grader_status ", this->get_status());
    return 1;
  }
  
  this->logger("grader is started");
  this->status = GRADER_STATUS::GRADER_STATUS_IDLE;
  
  while (
    this->get_status() != GRADER_STATUS::GRADER_STATUS_ERROR
    || this->get_status() != GRADER_STATUS::GRADER_STATUS_TERMINATED
  ) {
  
    // if there is a new submission
    if (this->get_status() == GRADER_STATUS::GRADER_STATUS_NEW) {
      this->status = GRADER_STATUS::GRADER_STATUS_BUSY;
      this->logger("submission", this->submission->get_submission_id()," is being graded");
      GRADE_RESULT result = grade_submission(this->get_grader_id(), this->submission);
      this->logger("finish grading submission ", this->submission->get_submission_id(), this->get_status());
      this->status = GRADER_STATUS::GRADER_STATUS_IDLE;
    }
    
    usleep(1000);
  }
  
  return 0;
}
