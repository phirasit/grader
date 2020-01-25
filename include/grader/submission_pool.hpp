//
// Created by phirasit on 12/28/19.
//

#ifndef GRADER_SUBMISSION_POOL_HPP
#define GRADER_SUBMISSION_POOL_HPP

#include "submission.hpp"

#include <map>
#include <optional>
#include <queue>

class SubmissionPool {
private:
    std::queue<Submission*> order;
    std::map<SubmissionID, Submission*> index;
    
public:
    SubmissionPool() = default;
    ~SubmissionPool() {
      for (const auto& it : index) {
        delete it.second;
      }
    }
    
    void add(Submission* submission);
    void remove(const SubmissionID& submission_id);
    
    [[nodiscard]] std::optional<Submission*> get_first();
    [[nodiscard]] std::optional<Submission*> get(const SubmissionID& submission_id) const;
    
    [[nodiscard]] size_t size() const { return this->order.size(); }
};


#endif //GRADER_SUBMISSION_POOL_HPP
