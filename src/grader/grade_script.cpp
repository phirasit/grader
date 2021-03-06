//
// Created by phirasit on 12/24/19.
//
#include "file.hpp"
#include "grader/grader.hpp"
#include "task.hpp"
#include "submission.hpp"

#include <error.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mount.h>

static Logger get_logger(const int grader_id) {
  return Logger ("worker" + std::to_string(grader_id) + "-workspace");
}

static inline file::File get_grader_tmpfs(const int grader_id) {
  return file::get_path("/grader/") + std::to_string(grader_id);
}
static inline std::string get_workspace_path(const int grader_id) {
  return file::get_path("/run/") + std::to_string(grader_id);
}
static inline file::File get_result_path(const SubmissionID& submission_id) {
  return file::get_path("/results/") + submission_id;
}

static GRADE_RESULT judge_submission(
  const Submission* submission,
  const Task& task,
  const Script& task_option,
  int grader_id) {
  const Logger logger = get_logger(grader_id);
  const file::File& workspace = get_workspace_path(grader_id);
  
  // copy submission to workspace
  const file::File& workspace_submission = workspace + "/" + task.get_submission_file();
  file::copy_file(submission->get_filename(), workspace_submission);
  
  // execute command inside a workspace
  // TODO handle errors
  task.get_prejudge_script().execute(workspace);
  task_option.execute(workspace);
  task.get_postjudge_script().execute(workspace);
  
  // report grading result
  const std::string& result_source_path = workspace + "/" + task.get_result_path();
  const std::string& result_target_path = get_result_path(submission->get_submission_id());
  if (file::copy_file(result_source_path, result_target_path) > 0) {
    logger("cannot copy result file");
    return GRADE_RESULT::CLEANUP_ERROR;
  }
  
  logger("finished judging successfully");
  return GRADE_RESULT::OK;
}

static int create_new_grading_workspace(const Task& task, const int grader_id) {
  const Logger logger = get_logger(grader_id);
  const file::File& workspace = get_workspace_path(grader_id);
  const file::File& upperdir = get_grader_tmpfs(grader_id) + "/workspace";
  const file::File& workdir = get_grader_tmpfs(grader_id) + "/workdir";
  
  // create folder
  file::create_folder(upperdir);
  file::create_folder(workdir);
  
  // mount overlayfs with task as lowerdir and tmpfs as upperdir and workdir
  const std::string& mount_option =
    "lowerdir=" + task.get_task_location() + "," +
    "upperdir=" + upperdir + "," +
    "workdir=" + workdir;
  if (mount("overlay", workspace.c_str(), "overlay", MS_MGC_VAL, mount_option.c_str())) {
    logger("[error] cannot mount workspace: ", strerror(errno));
    return 1;
  }
  
  logger("finish creating a new workspace");
  
  return 0;
}

static int cleanup_workspace(int grader_id) {
  const Logger logger = get_logger(grader_id);
  const file::File& grader = get_grader_tmpfs(grader_id);
  const file::File& workspace = get_workspace_path(grader_id);
  
  // unmount workspace
  if (umount(workspace.c_str())) {
    logger("cannot unmount: ", workspace);
    return 1;
  }
  
  // remove everything in the folder
  file::remove_folder(grader);
  file::create_folder(grader);
  
  logger("finished cleanup workspace");
  
  return 0; // no error
}

GRADE_RESULT grade_submission(const int grader_id, Submission* submission) {
  // load task
  std::optional<Task> loaded_task = Task::load_task(submission->get_task_id());
  if (!loaded_task.has_value()) {
    return GRADE_RESULT::NO_TASK;
  }
  const Task& task = loaded_task.value();
  if (task.get_task_version() != submission->get_task_version()) {
    // submission wants the different version of the task
    return GRADE_RESULT::INVALID_VERSION;
  }
  std::optional<Script> judge_script = task.get_judge_option(submission->get_option());
  if (!judge_script.has_value()) {
    // the submission requires the non-existing grading version
    return GRADE_RESULT::INVALID_OPTION;
  }
  const Script& option = judge_script.value();
  
  // setup environment
  if (create_new_grading_workspace(task, grader_id)) {
    // fail to create grading environment
    return GRADE_RESULT::SETUP_ERROR;
  }
  
  const GRADE_RESULT judge_result = judge_submission(submission, task, option, grader_id);
  
  // cleanup environment
  if (cleanup_workspace(grader_id)) {
    return GRADE_RESULT::CLEANUP_ERROR;
  }
  
  return judge_result;
}

