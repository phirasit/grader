#ifndef __GRADE_HPP__
#define __GRADE_HPP__

#include "submission.hpp"
#include "logger.hpp"

#include <ostream>

enum GRADE_RESULT {
    OK = 0,
    NO_TASK,
    INVALID_VERSION,
    INVALID_OPTION,
    SETUP_ERROR,
    JUDGE_ERROR,
    CLEANUP_ERROR,
};

std::string to_string(GRADE_RESULT status);
std::ostream& operator << (std::ostream& out, GRADE_RESULT result);

enum GRADER_STATUS {
    GRADER_STATUS_IDLE,
    GRADER_STATUS_INIT,
    GRADER_STATUS_NEW,
    GRADER_STATUS_BUSY,
    GRADER_STATUS_DONE,
    GRADER_STATUS_TERMINATED,
    GRADER_STATUS_ERROR,
};

std::string to_string(GRADER_STATUS status);
std::ostream& operator << (std::ostream& out, GRADER_STATUS status);

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
    Submission* get_submission() const { return this->submission; }
    
    bool is_terminated() const {
      return this->status == GRADER_STATUS_ERROR || this->status == GRADER_STATUS_TERMINATED;
    }
    
    void grade(Submission* submission);
    
    void signal(GRADER_SIGNAL sig);
    
    int start_grader();
};

GRADE_RESULT grade_submission(int grader_id, Submission* submission);

#endif // __GRADE_HPP__
