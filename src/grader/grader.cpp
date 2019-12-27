#include "file.hpp"
#include "grader.hpp"
#include "task.hpp"

#include <sys/mount.h>
#include <unistd.h>

#include <iostream>

void Grader::grade(const Submission& submission) {
  if (this->get_status() == GRADER_STATUS::IDLE) {
    this->submission = &submission;
    this->status = GRADER_STATUS::NEW;
  }
}

static int init_grader(const int id) {
  file::create_folder(file::get_path("/grader") + std::to_string(id));
  file::create_folder(file::get_path("/run") + std::to_string(id));
}

static int cleanup_grader(const int id) {
  file::remove_folder(file::get_path("/run") + std::to_string(id));
  file::remove_folder(file::get_path("/grader") + std::to_string(id));
}

static void terminate(Grader* grader) {
  log(grader->get_grader_id(), "terminating...");
  if (grader->get_status() == GRADER_STATUS::INIT) {
    return;
  }
  cleanup_grader(grader->get_grader_id());
}

void Grader::signal(GRADER_SIGNAL sig) {
  log(this->get_grader_id(), "receive signal " + std::to_string(sig));
  switch (sig) {
    case GRADER_SIGNAL::KILL:
      terminate(this);
      this->status = GRADER_STATUS::TERMINATED;
      break;
    default:
      break;
  }
}

void Grader::start_grader() {
  log(this->get_grader_id(), "start grader");
  
  this->status = GRADER_STATUS::INIT;
  if (init_grader(this->get_grader_id())) {
    this->status = GRADER_STATUS::ERROR;
    return;
  }
  
  this->status = GRADER_STATUS::IDLE;
  
  while (
    this->get_status() != GRADER_STATUS::ERROR
    || this->get_status() != GRADER_STATUS::TERMINATED
  ) {
  
    // if there is a new submission
    if (this->get_status() == GRADER_STATUS::NEW) {
      this->status = GRADER_STATUS::BUSY;
      GRADE_RESULT result = grade_submission(this->get_grader_id(), *this->submission);
      this->status = GRADER_STATUS::IDLE;
    }
    
    sleep(1);
  }
  
}
