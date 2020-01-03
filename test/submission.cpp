//
// Created by phirasit on 1/3/20.
//
#include "catch.hpp"
#include "yaml-cpp/yaml.h"

#include "submission.hpp"

Submission load_sample_submission() {
  const YAML::Node &submission_yaml = YAML::LoadFile("test/sample-submission/config.yaml");
  REQUIRE(submission_yaml);
  std::optional<Submission> submission_obj = Submission::from_yaml(submission_yaml, "test/sample-submission/solution.cpp");
  REQUIRE(submission_obj.has_value());
  return submission_obj.value();
}

TEST_CASE("submission parsing", "[submission]") {
  // load sample submission
  const Submission submission = load_sample_submission();
  
  REQUIRE(submission.get_submission_id() == "sample-submission-1");
  REQUIRE(submission.get_task_id() == "a+b");
  REQUIRE(submission.get_task_version() == "1");
  REQUIRE(submission.get_option() == "default");
}
