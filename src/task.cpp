#include "task.hpp"

#ifndef DEFAULT_OPTION_KEY
#define DEFAULT_OPTION_KEY "default"
#endif

const file::File Task::IGNORE_LOCATION = "";

std::optional<Script> Task::get_judge_option(const std::string &option) const {
  const MapStringScript& options = this->judge_options;
  // get key = option
  const auto it = options.find(option);
  if (it != options.end()) return it->second;
  // get default option
  const auto default_it = options.find(DEFAULT_OPTION_KEY);
  if (it != options.end()) return default_it->second;
  // no option found
  return std::nullopt;
}

std::optional<Task> Task::load_task(const std::string& task_id) {
  const file::File& task_path = file::get_path("/tasks/") + task_id;
  
  if (file::file_readable(task_path + std::string("/config.yaml"))){ // read from yaml
    const YAML::Node& yaml = YAML::LoadFile(task_path + std::string("/config.yaml"));
    return Task::from_yaml(yaml, task_path);
  }
  
  return std::nullopt;
}

// parse yaml to Task object
// TODO support error logging
std::optional<Task> Task::from_yaml(const YAML::Node& yaml, const file::File& location) {
  if (!yaml.IsMap()) return std::nullopt;
  
  // get task id
  if (!yaml["id"]) return std::nullopt;
  const TaskID& task_id = TaskID(yaml["id"].as<std::string>());
  
  // get task version
  if (!yaml["version"]) return std::nullopt;
  const std::string& task_version = yaml["version"].as<std::string>();
  
  // get task install script
  const std::optional<Script> install_script = yaml["install"]
    ? Script::from_yaml(yaml["install"])
    : Script();
  if (!install_script.has_value()) return std::nullopt;
  
  // get task prejudge script
  const std::optional<Script> prejudge = yaml["prejudge"]
    ? Script::from_yaml(yaml["prejudge"])
    : Script();
  if (!prejudge.has_value()) return std::nullopt;
  
  // get task options
  const YAML::Node &option_yaml = yaml["option"];
  if (option_yaml.IsNull()) return std::nullopt;
  if (!option_yaml.IsMap()) return std::nullopt;
  
  MapStringScript options;
  const std::optional<Script> default_script = Script::from_yaml(option_yaml);
  if (default_script.has_value()) {
    options.insert({DEFAULT_OPTION_KEY, default_script.value()});
  } else {
    for (const auto &key : option_yaml) {
      const std::optional<Script> script = Script::from_yaml(key.second);
      if (!script.has_value()) {
        return std::nullopt;
      } else {
        options.insert({key.first.as<std::string>(), script.value()});
      }
    }
  }
  
  // get task postjudge script
  const std::optional<Script> postjudge = yaml["postjudge"]
    ? Script::from_yaml(yaml["postjudge"])
    : Script();
  if (!postjudge.has_value()) return std::nullopt;
  
  // get task result file
  if (!yaml["result"]) return std::nullopt;
  const std::string& result_file = yaml["result"].as<std::string>();
  
  return Task(
    location, task_id, task_version,
    install_script.value(), prejudge.value(), options, postjudge.value(),
    result_file);
}