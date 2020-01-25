//
// Created by phirasit on 12/28/19.
//

#include "grader/submission_pool.hpp"

void SubmissionPool::add(Submission* submission) {
  this->order.push(submission);
  this->index.insert({submission->get_submission_id(), submission});
}

void SubmissionPool::remove(const SubmissionID& submission_id) {
  this->index.erase(submission_id);
}

std::optional<Submission*> SubmissionPool::get_first() {
  Submission* first_sub;
  
  do {
    if (this->order.empty()) {
      return std::nullopt;
    }
    first_sub = this->order.front();
    this->order.pop();
  } while (this->index.find(first_sub->get_submission_id()) != this->index.end());
  
  return first_sub;
}

std::optional<Submission*> SubmissionPool::get(const SubmissionID& submission_id) const {
  if (this->index.find(submission_id) != this->index.end()) {
    return this->index.at(submission_id);
  }
  return std::nullopt;
}

