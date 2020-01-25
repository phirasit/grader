#include "file.hpp"
#include "logger.hpp"

#include <iostream>

int main(int argc, char *argv[]) {
  const Logger logger ("setup");
  
  logger("start setting up grader...");
  logger("root directory is ", file::get_path(""));
  
  // create all directories
  file::create_folder(file::get_path("/"));
  file::create_folder(file::get_path("/tasks"));
  file::create_folder(file::get_path("/results"));
  file::create_folder(file::get_path("/submissions"));
  
  file::create_folder("/tmp/grader");
  file::remove_file(file::get_path("/grader"));
  file::create_symlink("/tmp/grader", file::get_path("/grader"));
  
  file::create_folder("/tmp/grader/run");
  file::remove_file(file::get_path("/run"));
  file::create_symlink("/tmp/grader/run", file::get_path("/run"));
  
  logger("finish setting up grader...");
  
  return 0;
}
