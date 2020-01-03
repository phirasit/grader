#ifndef __GRADE_HPP__
#define __GRADE_HPP__

#include "submission.hpp"
#include "logger.hpp"

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
    GRADER_STATUS_IDLE,
    GRADER_STATUS_INIT,
    GRADER_STATUS_NEW,
    GRADER_STATUS_BUSY,
    GRADER_STATUS_TERMINATED,
    GRADER_STATUS_ERROR,
};

enum GRADER_SIGNAL {
    KILL,
};

class Grader {
private:
    const int grader_id;
    const Logger logger;
    pthread_t thread_id;
    GRADER_STATUS status;
    
    Submission* submission;
    
public:
    
    Grader (int id) :
      grader_id(id), logger("worker" + std::to_string(grader_id)), thread_id(0),
      status(GRADER_STATUS_INIT), submission(nullptr) {}
    ~Grader() = default;
    
    inline int get_grader_id() { return this->grader_id; }
    inline pthread_t& get_thread_id() { return this->thread_id; }
    inline GRADER_STATUS get_status() const { return this->status; }
    
    void grade(Submission* submission);
    
    void signal(GRADER_SIGNAL sig);
    
    int start_grader();
};

GRADE_RESULT grade_submission(int grader_id, Submission* submission);

#endif // __GRADE_HPP__
