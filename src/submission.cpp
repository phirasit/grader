//
// Created by phirasit on 1/3/20.
//
#include "submission.hpp"
#include <optional>

bool is_map(const YAML::Node& root) { return root.IsMap(); }
bool is_map(const Json::Value& root) { return root.isObject(); }

template <typename T>
T get(const YAML::Node& root) {
  return root.as<T>();
}

template <typename T>
T get(const Json::Value& root) {
  if constexpr (std::is_same<T, std::string>::value) {
    return root.asString();
  }
}

// TODO support error report
template<typename T>
static std::optional<Submission> create_submission_from_description(const T &yaml, const file::File& filename) {
  if (!is_map(yaml)) return std::nullopt;
  
  if (!yaml["id"]) return std::nullopt;
  const SubmissionID& submission_id = get<std::string>(yaml["id"]);
  
  if (!yaml["task-id"]) return std::nullopt;
  const TaskID& task_id = get<std::string>(yaml["task-id"]);
  
  if (!yaml["task-version"]) return std::nullopt;
  const std::string& task_version = get<std::string>(yaml["task-version"]);
  
  if (!yaml["option"]) return std::nullopt;
  const std::string& option = get<std::string>(yaml["option"]);
  
  return Submission(submission_id, task_id, task_version, option, filename);
}

std::optional<Submission> Submission::from_yaml(const YAML::Node &yaml, const file::File& filename) {
  return create_submission_from_description<YAML::Node>(yaml, filename);
}

std::optional<Submission> Submission::from_json(const Json::Value &json, const file::File& filename) {
  return create_submission_from_description<Json::Value>(json, filename);
}
