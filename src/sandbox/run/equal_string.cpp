//
// Created by phirasit on 1/16/20.
//

#include "equal_string.hpp"
#include "logger.hpp"

#include <fcntl.h>
#include <fstream>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>

#include <ext/stdio_filebuf.h>

bool RunConfigCompareStringEqual::is_equal(const std::string& a, const std::string& b) const {
  return a == b;
}

GRADE_STATUS RunConfigCompareStringEqual::grade(int test_id) const {
  static const Logger logger("grade-result");
  const std::string& filename = std::to_string(test_id);
  logger("start grading... #", test_id);
  
  // create a pipe between checker and program
  int fd[2];
  if (pipe(fd) == -1) {
    logger("cannot create a pipe");
    return GRADE_STATUS_SYSTEM_ERROR;
  }
  
  // fork a child process
  pid_t pid = fork();
  if (pid < 0) {
    logger("cannot fork a process");
    return GRADE_STATUS_SYSTEM_ERROR;
  }
  
  ///////// child process /////////
  if (pid == 0) {
    // close read fd
    close(fd[0]);
    // bind input file to stdin
    int inp_fd = open(this->in_file.get_string(filename).c_str(), O_RDONLY);
    if (!inp_fd) exit(1);
    dup2(inp_fd, STDIN_FILENO);
    // bind stdout to write fd
    dup2(fd[1], STDOUT_FILENO);
    // close stderr
    close(STDERR_FILENO);
  
    // TODO link stdin and stdout
    this->execute_script();
    exit(0);
  }

  //////// checker //////////
  // close write fd and bind read from read fd
  close(fd[1]);
  
  // open solution file
  std::ifstream sol (this->sol_file.get_string(filename));
  if (!sol.is_open()) {
    logger("cannot open ", this->sol_file.get_string(filename));
    close(fd[0]);
    return GRADE_STATUS_SYSTEM_ERROR;
  }
  
  // create istream from read fd
  __gnu_cxx::stdio_filebuf<char> filebuf(fd[0], std::ios::in);
  std::istream out (&filebuf);
  
  // wait for child process to finish grading
  int wstatus;
  waitpid(pid, &wstatus, 0);
  
  GRADE_STATUS result = RunConfig::interpret_wstatus(wstatus);
  
  logger("start checking result: ", test_id);
  // check result
  if (result == GRADE_STATUS_OK) {
    while ((!out.eof() || !sol.eof())) {
      std::string output, target;
      out >> output;
      sol >> target;
      logger("comparing -- \"", output, "\" # \"", target, "\"");
      if (!this->is_equal(output, target)) {
        result = GRADE_STATUS_FAILED;
        break;
      }
    }
  }
  
  if (out.eof() != sol.eof()) {
    result = GRADE_STATUS_FAILED;
  }
  
  sol.close();
  close(fd[0]);
  
  logger("test result ", test_id, " = ", result);
  
  return result;
}
