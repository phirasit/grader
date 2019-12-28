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
    
    void add(Submission* submission);
    std::optional<Submission*> get_first();
    std::optional<Submission*> get(const SubmissionID& submission_id) const;
    
    size_t size() const { return this->order.size(); }
};


#endif //GRADER_SUBMISSION_POOL_HPP
