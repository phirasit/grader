//
// Created by phirasit on 1/18/20.
//

#include "run.hpp"

#include <sys/resource.h>
#include <sys/prctl.h>

#include <seccomp.h>

int RunConfig::enter_sandbox() const {
  if (prctl(PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0) == -1) {
    return 1;
  }
  
  // setup system call rules
  scmp_filter_ctx ctx;
  ctx = seccomp_init(SCMP_ACT_KILL);
  
  // basic whitelist functions
  seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(access), 0);
  seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(arch_prctl), 0);
  seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(brk), 0);
  seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(close), 0);
  seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(clock_gettime), 0);
  seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(execve), 0);
  seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(exit), 0);
  seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(exit_group), 0);
  seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(fstat), 0);
  seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(lseek), 0);
  seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(read), 0);
  seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(readlink), 0);
  seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(rt_sigreturn), 0);
  seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(sigreturn), 0);
  seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(write), 0);
  seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(uname), 0);
  ///// Add more rules here
  
  
  // setup memory limit
  uint64_t memory_limit_byte = this->constraint.get_memory_mb() << 20;
  struct rlimit memory_limit = {
    .rlim_cur = memory_limit_byte,
    .rlim_max = memory_limit_byte,
  };
  if (setrlimit(RLIMIT_AS, &memory_limit) == -1) {
    return 1;
  }
  if (setrlimit(RLIMIT_DATA, &memory_limit) == -1) {
    return 1;
  }
  if (setrlimit(RLIMIT_STACK, &memory_limit) == -1) {
    return 1;
  }
  
  // setup time limit
  uint64_t time_limit_second = ((uint64_t) this->constraint.get_time_ms() + 999) / 1000;
  struct rlimit cpu_limit = {
    .rlim_cur = time_limit_second,
    .rlim_max = time_limit_second + 1,
  };
  if (setrlimit(RLIMIT_CPU, &cpu_limit) == -1) {
    return 1;
  }
  
  // load system call rules
  seccomp_load(ctx);
  
  return 0;
}

