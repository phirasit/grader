//
// Created by phirasit on 12/28/19.
//

#include "grader/submission_pool.hpp"

void SubmissionPool::add(Submission* submission) {
  this->order.push(submission);
  this->index.insert({submission->get_submission_id(), submission});
}

std::optional<Submission*> SubmissionPool::get_first() {
  if (this->order.empty()) {
    return std::nullopt;
  }
  Submission* first_sub = this->order.front();
  this->order.pop();
  this->index.erase(first_sub->get_submission_id());
  return first_sub;
}

std::optional<Submission*> SubmissionPool::get(const SubmissionID& submission_id) const {
  if (this->index.find(submission_id) != this->index.end()) {
    return this->index.at(submission_id);
  }
  return std::nullopt;
}

