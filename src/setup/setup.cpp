#include "file.hpp"
#include <iostream>

int main(int argc, char *argv[]) {

  std::cerr << "setting up Grader..." << std::endl;
  std::cerr << "root directory is " << file::get_path("") << std::endl;
  
  // create all directories
  file::create_folder(file::get_path("/"));
  file::create_folder(file::get_path("/tasks"));
  file::create_folder(file::get_path("/results"));
  
  file::remove_file(file::get_path("/grader"));
  file::create_folder("/tmp/grader");
  file::create_symlink("/tmp/grader", file::get_path("/grader"));
  
  file::remove_file(file::get_path("/run"));
  file::create_folder("/tmp/grader/run");
  file::create_symlink("/tmp/grader/run", file::get_path("/run"));
  
  std::cerr << "finish setting up Grader..." << std::endl;
  
  return 0;
}
