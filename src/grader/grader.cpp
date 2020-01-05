#include "grader/grader.hpp"

#include "file.hpp"
#include "task.hpp"

#include <sys/mount.h>
#include <unistd.h>

std::ostream& operator << (std::ostream& out, GRADE_RESULT result) {
  switch (result) {
    case OK: return out << "OK";
    case NO_TASK: return out << "NO_TASK";
    case INVALID_VERSION: return out << "INVALID_VERSION";
    case INVALID_OPTION: return out << "INVALID_OPTION";
    case SETUP_ERROR: return out << "SETUP_ERROR";
    case JUDGE_ERROR: return out << "JUDGE_ERROR";
    case CLEANUP_ERROR: return out << "CLEANUP_ERROR";
    default: return out << "INVALID_GRADE_RESULT";
  }
}

std::ostream& operator << (std::ostream& out, GRADER_STATUS status) {
  switch (status) {
    case GRADER_STATUS_IDLE: return out << "IDLE";
    case GRADER_STATUS_INIT: return out << "INIT";
    case GRADER_STATUS_NEW: return out << "NEW";
    case GRADER_STATUS_BUSY: return out << "BUSY";
    case GRADER_STATUS_DONE: return out << "DONE";
    case GRADER_STATUS_TERMINATED: return out << "TERMINATED";
    case GRADER_STATUS_ERROR: return out << "ERROR";
    default: return out << "INVALID_GRADER_STATUS";
  }
}

void Grader::grade(Submission* submission) {
  switch (this->get_status()) {
    case GRADER_STATUS_IDLE:
      this->submission = submission;
      this->logger("new submission: ", submission->get_submission_id());
      this->status = GRADER_STATUS_NEW;
      break;
    case GRADER_STATUS_DONE:
      if (submission == nullptr) {
        this->status = GRADER_STATUS_IDLE;
      }
    default:
      break;
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
      this->status = GRADER_STATUS_TERMINATED;
      this->logger("terminated");
      break;
    default:
      break;
  }
}

int Grader::start_grader() {
  this->logger("starting worker...");
  
  this->status = GRADER_STATUS_INIT;
  if (init_grader(this->get_grader_id())) {
    this->logger("worker is failed to start");
    this->status = GRADER_STATUS_ERROR;
    return 1;
  }
  
  this->logger("worker is started");
  this->status = GRADER_STATUS_IDLE;
  
  while (!this->is_terminated()) {
    // if there is a new submission
    if (this->get_status() == GRADER_STATUS_NEW) {
      this->status = GRADER_STATUS_BUSY;
      this->logger("submission: \"", this->submission->get_submission_id(), "\" is being graded");
      GRADE_RESULT result = grade_submission(this->get_grader_id(), this->submission);
      this->logger("finish grading submission \"", this->submission->get_submission_id(), "\" status ", result);
      this->status = GRADER_STATUS_DONE;
    }
    
    usleep(1000);
  }
  
  return 0;
}
