//
// Created by phirasit on 12/24/19.
//

#ifndef GRADER_SCRIPT_HPP
#define GRADER_SCRIPT_HPP

#include "yaml-cpp/yaml.h"
#include "file.hpp"

#include <string>
#include <optional>
#include <variant>
#include <vector>

class Script {
private:
    const std::vector<std::string> scripts;
    
public:
    Script(std::initializer_list<std::string> scripts) : scripts(scripts) {}
    Script(std::vector<std::string> scripts) : scripts(std::move(scripts)) {}
    
    Script() = default;
    Script(const Script& script) : Script(script.scripts) {}
    ~Script() = default;
    
    bool empty() const { return scripts.empty(); }
    int execute(const file::File& path) const;
    
    static std::optional<Script> from_yaml(const YAML::Node& yaml);
};

#endif //GRADER_SCRIPT_HPP
