//
// Created by phirasit on 12/24/19.
//

#ifndef GRADER_TASK_HPP
#define GRADER_TASK_HPP

#include "file.hpp"
#include "script.hpp"
#include "yaml-cpp/yaml.h"

#include <map>
#include <optional>

using TaskID = std::string;
using MapStringScript = std::map<std::string, Script>;

class Task {
private:
    const file::File location;
    const TaskID task_id;
    const std::string task_version;
    
    const Script install_script;
    const Script prejudge;
    const MapStringScript judge_options;
    const Script postjudge;
    
    const std::string submission_file;
    const std::string result_file;
    
public:
    
    static const file::File IGNORE_LOCATION;
    
    Task(file::File location,
      TaskID task_id,
      std::string task_version,
      const Script& install,
      const Script& prejudge,
      MapStringScript judge_options,
      const Script& postjudge,
      std::string submission_file,
      std::string result_file)
      : location(std::move(location)), task_id(std::move(task_id)), task_version(std::move(task_version)),
        install_script(install),
        prejudge(prejudge), judge_options(std::move(judge_options)), postjudge(postjudge),
        submission_file(std::move(submission_file)), result_file(std::move(result_file)) {}
    ~Task() = default;
    
    const file::File& get_task_location() const { return this->location; }
    const TaskID& get_task_id() const { return this->task_id; }
    const std::string& get_task_version() const { return this->task_version; }
    const std::string& get_submission_file() const { return this->submission_file; }
    const std::string& get_result_path() const { return this->result_file; }
    
    const Script& get_install_script() const { return this->install_script; }
    const Script& get_prejudge_script() const { return this->prejudge; }
    std::optional<Script> get_judge_option(const std::string& option) const;
    const Script& get_postjudge_script() const { return this->postjudge; }
    
    static std::optional<Task> load_task(const std::string& task_id);
    
    static std::optional<Task> from_yaml(const YAML::Node& yaml, const file::File& location);
    
    /*
    // TODO support constructor from data object
    static Task from_json(JSON);
    static Task from_mysql(MYSQL);
    */
};

#endif //GRADER_TASK_HPP
