//
// Created by phirasit on 12/24/19.
//

#ifndef GRADER_TASK_HPP
#define GRADER_TASK_HPP

#include "file.hpp"
#include "script.hpp"
#include "yaml-cpp/yaml.h"

#include <map>

using MapStringScript = std::map<std::string, Script>;

class Task {
private:
    const std::string location;
    const std::string task_id;
    const std::string task_version;
    
    const Script install_script;
    const Script prejudge;
    const MapStringScript judge_options;
    const Script postjudge;
    
    const std::string result_file;
    
public:
    
    Task(std::string location,
      std::string task_id,
      std::string task_version,
      const Script& prejudge,
      MapStringScript judge_options,
      const Script& postjudge,
      std::string result_file)
      : location(std::move(location)), task_id(std::move(task_id)), task_version(std::move(task_version)),
        prejudge(prejudge), judge_options(std::move(judge_options)), postjudge(postjudge),
        result_file(std::move(result_file)) {}
    ~Task() = default;
    
    const std::string& get_task_location() const { return this->location; }
    const std::string& get_task_id() const { return this->task_id; }
    const std::string& get_task_version() const { return this->task_version; }
    const std::string& get_result_path() const { return this->result_file; }
    
    std::optional<Script> get_judge_option(const std::string& option) const;
    
    static std::optional<Task> load_task(const std::string& task_id);
    
    static std::optional<Task> from_yaml(const YAML::Node& yaml, const file::File& location);
    
    /*
    // TODO support constructor from data object
    static Task from_json(JSON);
    static Task from_mysql(MYSQL);
    */
};

#endif //GRADER_TASK_HPP
