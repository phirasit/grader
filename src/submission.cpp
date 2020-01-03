//
// Created by phirasit on 1/3/20.
//
#include "submission.hpp"
#include <optional>

// TODO support error report
std::optional<Submission> Submission::from_yaml(const YAML::Node &yaml, const file::File& filename) {
  if (!yaml.IsMap()) return std::nullopt;
  
  if (!yaml["id"]) return std::nullopt;
  const SubmissionID& submission_id = SubmissionID(yaml["id"].as<std::string>());
  
  if (!yaml["task-id"]) return std::nullopt;
  const TaskID& task_id = TaskID(yaml["task-id"].as<std::string>());
  
  if (!yaml["task-version"]) return std::nullopt;
  const std::string& task_version = yaml["task-version"].as<std::string>();
  
  if (!yaml["option"]) return std::nullopt;
  const std::string& option = yaml["option"].as<std::string>();
  
  return Submission(submission_id, task_id, task_version, option, filename);
}
