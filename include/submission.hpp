//
// Created by phirasit on 12/23/19.
//

#ifndef GRADER_SUBMISSION_HPP
#define GRADER_SUBMISSION_HPP

#include <istream>

using SubmissionID = std::string;

class Submission {
private:
    const SubmissionID submission_id;
    const std::string task_id;
    const std::string task_version;
    const std::string grading_option;
    const std::string filename;
    
public:
    
    const SubmissionID& get_submission_id() const { return this->submission_id; }
    const std::string& get_task_id() const { return this->task_id; }
    const std::string& get_task_version() const { return this->task_version; }
    const std::string& get_option() const { return this->grading_option; }
    const std::string& get_filename() const { return this->filename; }
    
    /*
    // TODO add multiple source support
    static Submission load_yaml();
    static Submission load_json();
    static Submission load_mysql();
    */
};
#endif //GRADER_SUBMISSION_HPP
