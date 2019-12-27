#ifndef __GRADE_HPP__
#define __GRADE_HPP__

#include "submission.hpp"

#include <iostream>

enum GRADE_RESULT {
    OK = 0,
    
    NO_TASK,
    INVALID_VERSION,
    INVALID_OPTION,
    SETUP_ERROR,
    PREJUDGE_ERROR,
    POSTJUDGE_ERROR,
    CLEANUP_ERROR,
};

enum GRADER_STATUS {
    IDLE,
    INIT,
    NEW,
    BUSY,
    TERMINATED,
    ERROR,
};

enum GRADER_SIGNAL {
    KILL,
};

class Grader {
private:
    const int grader_id;
    pthread_t thread_id;
    GRADER_STATUS status;
    
    const Submission* submission;
    
public:
    
    Grader (int id) : grader_id(id), thread_id(0), status(GRADER_STATUS::INIT), submission(nullptr) {}
    
    inline int get_grader_id() { return this->grader_id; }
    inline pthread_t& get_thread_id() { return this->thread_id; }
    inline GRADER_STATUS get_status() const { return this->status; }
    
    void grade(const Submission& submission);
    
    void signal(GRADER_SIGNAL sig);
    
    void start_grader();
};

GRADE_RESULT grade_submission(int grader_id, const Submission& submission);

static void log(const int grader_id, const std::string& msg) {
  std::cerr << "[grader " << grader_id << "] " << msg << std::endl;
}

#endif // __GRADE_HPP__
