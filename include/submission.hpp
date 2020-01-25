//
// Created by phirasit on 12/23/19.
//

#ifndef GRADER_SUBMISSION_HPP
#define GRADER_SUBMISSION_HPP

#include "json/json.h"
#include "yaml-cpp/yaml.h"
#include "task.hpp"
#include "file.hpp"

#include <optional>

using SubmissionID = std::string;

class Submission {
private:
    const SubmissionID submission_id;
    const TaskID task_id;
    const std::string task_version;
    const std::string grading_option;
    const file::File filename;
    
public:
    Submission(SubmissionID submission_id, TaskID task_id,
      std::string version, std::string option, file::File filename)
      : submission_id(std::move(submission_id)), task_id(std::move(task_id)),
      task_version(std::move(version)), grading_option(std::move(option)),
      filename(std::move(filename)) {}
      
    [[nodiscard]] const SubmissionID& get_submission_id() const { return this->submission_id; }
    [[nodiscard]] const TaskID& get_task_id() const { return this->task_id; }
    [[nodiscard]] const std::string& get_task_version() const { return this->task_version; }
    [[nodiscard]] const std::string& get_option() const { return this->grading_option; }
    [[nodiscard]] const file::File& get_filename() const { return this->filename; }
    
    static std::optional<Submission> from_yaml(const YAML::Node &yaml, const file::File& filename);
    static std::optional<Submission> from_json(const Json::Value &json, const file::File& filename);
    
    /*
    // TODO add multiple source support
    static Submission load_mysql();
    */
};

#endif //GRADER_SUBMISSION_HPP
