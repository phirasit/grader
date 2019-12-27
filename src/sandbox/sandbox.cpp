/*
 * Run a command inside a seccomp process
 */
#include <linux/seccomp.h>
#include <sys/prctl.h>
#include <errno.h>

#include <iostream>
#include <unistd.h>

int main(int argc, char* argv[]) {
  if (argc <= 1) {
    std::cerr << "usage: sandbox program [args...]" << std::endl;
    return 1;
  }

  // set mode to seccomp
  if (prctl(PR_SET_SECCOMP, SECCOMP_MODE_STRICT) == -1) {
    std::cerr << "failed to set seccomp" << std::endl;
    std::cerr << "errno code: " << errno << std::endl;
    return 1;
  }

  // TODO limit processing time

  // TODO limit amount of process memory

  // execute the command
  execvp(argv[1], argv + 1);

  return 0;
}
