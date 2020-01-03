//
// Created by phirasit on 1/2/20.
//

#ifndef GRADER_LOGGER_HPP
#define GRADER_LOGGER_HPP

#include <mutex>
#include <iostream>
#include <string>

static std::mutex g_log_mutex;

class Logger {
private:
    std::string prefix;
    
public:
    Logger (std::string prefix) : prefix(std::move(prefix)) {}
    
    template<typename ...Args>
    std::ostream& operator () (Args&& ...args) const {
      ::g_log_mutex.lock();
      std::cerr << "[" << this->prefix << "] ";
      (std::cerr << ... << args) << std::endl;
      ::g_log_mutex.unlock();
      return std::cerr;
    }
};

#endif //GRADER_LOGGER_HPP
