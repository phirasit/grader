//
// Created by phirasit on 12/28/19.
//

#ifndef GRADER_GRADER_POOL_HPP
#define GRADER_GRADER_POOL_HPP

#include "grader/submission_pool.hpp"
#include "grader/grader.hpp"

#include <queue>
#include <map>
#include <vector>
#include <optional>
#include <functional>

using OnFinishGradingType = std::function<void(const Submission&)>;

enum GRADER_POOL_STATUS {
    GRADER_POOL_INIT,
    GRADER_POOL_RUNNING,
    GRADER_POOL_TERMINATED,
};

namespace std {
    string to_string(GRADER_POOL_STATUS status);
}

class GraderPool {
private:
    const int num_graders;
    const Logger logger;
    SubmissionPool waiting_pool, graded_pool;
    std::vector<Grader*> graders;
    GRADER_POOL_STATUS status;
    
    void assign_submission_to_grader();
    
public:
    GraderPool (int num_graders) :
      num_graders(num_graders), logger("grader-pool"),
      waiting_pool(), graded_pool(), graders(),
      status(GRADER_POOL_INIT) {}
    ~GraderPool() {
      for (const Grader* grader : this->graders) {
        delete grader;
      }
    }
    
    int add_submission(const Submission& submission);
    [[nodiscard]] std::optional<Submission*> get_submission(const SubmissionID& submission_id) const;
    
    void start();
    void stop();
    void signal(GRADER_SIGNAL sig);
    
    [[nodiscard]] int get_num_graders() const { return this->num_graders; }
    [[nodiscard]] GRADER_POOL_STATUS get_status() const { return this->status; }
    
    [[nodiscard]] size_t get_waiting_pool_size() const { return this->waiting_pool.size(); }
    [[nodiscard]] size_t get_graded_pool_size() const { return this->graded_pool.size(); }
    [[nodiscard]] GRADER_STATUS get_grader_status(int grader_id) const {
      return this->graders.at(grader_id)->get_status();
    }
    
};

#endif //GRADER_GRADER_POOL_HPP
