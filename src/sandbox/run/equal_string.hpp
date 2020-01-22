//
// Created by phirasit on 1/16/20.
//

#ifndef GRADER_EQUAL_STRING_H
#define GRADER_EQUAL_STRING_H

#include "run.hpp"
#include "../utility.hpp"

class RunConfigCompareStringEqual : public RunConfig {
private:
    StringFormatter in_file, sol_file;
    
protected:
    virtual bool is_equal(const std::string& a, const std::string& b) const;
    
public:
    [[nodiscard]] static std::string get_name() { return "equal_string"; }
    
    void update(const YAML::Node& config) override {
      RunConfig::update(config);
      update_if_exists(in_file.get(), config["in"]);
      update_if_exists(sol_file.get(), config["sol"]);
    }
    
    [[nodiscard]] CaseResult grade(int test_id) const override;
};


#endif //GRADER_EQUAL_STRING_H
