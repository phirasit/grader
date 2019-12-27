//
// Created by phirasit on 12/24/19.
//
#include "file.hpp"

#include <error.h>
#include <string.h>
#include <stdlib.h>
#include <fstream>

#include <sys/stat.h>
#include <sys/mount.h>
#include <unistd.h>

#include <iostream>

#define Q(x) #x
#define QUOTE(x) Q(x)

#ifndef BASE_DIR
#define BASE_DIR "/"
#endif

namespace file {
    
    void change_folder(const File& path) {
      if (chdir(path.c_str())) {
        std::cerr << "[error] cannot change directory to " << path << std::endl;
        std::cerr << "\t" << strerror(errno) << std::endl;
      } else {
        std::cerr << "change directory to " << path << std::endl;
      }
    }
    
    void create_folder(const File& path) {
      if (mkdir(path.c_str(), 0770)) {
        if (errno != EEXIST) {
          std::cerr << "[error] cannot create a new folder " << path << std::endl;
          std::cerr << "\t" << strerror(errno) << std::endl;
        } else {
          std::cerr << "file exists " << path << std::endl;
        }
      } else {
        std::cerr << "create new folder " << path << std::endl;
      }
    }
    
    void remove_file(const File& path) {
      if (remove(path.c_str())) {
        std::cerr << "[error] cannot remove file " << path << std::endl;
        std::cerr << "\t" << strerror(errno) << std::endl;
      } else {
        std::cerr << "remove file " << path << std::endl;
      }
    }
    
    void remove_folder(const File& path) {
      if (unlink(path.c_str())) {
        std::cerr << "[error] cannot remove folder " << path << std::endl;
        std::cerr << "\t" << strerror(errno) << std::endl;
      } else {
        std::cerr << "remove folder " << path << std::endl;
      }
    }
    
    void create_symlink(const File& target, const File& link) {
      if (symlink(target.c_str(), link.c_str())) {
        std::cerr << "[error] cannot create symlink from " << link << " to " << target << std::endl;
        std::cerr << "\t" << strerror(errno) << std::endl;
      } else {
        std::cerr << "create symlink from " << link << " to " << target << std::endl;
      }
    }
    
    bool file_readable(const File& file) {
      return access(file.c_str(), R_OK) != -1;
    }
    
    File get_path(const File &path) {
      File base_dir = QUOTE(BASE_DIR);
      const char* base_dir_env = std::getenv("GRADER_BASE_DIR");
      if (base_dir_env != nullptr) {
        base_dir = File(base_dir_env);
      }
      return base_dir + path;
    }
    
    // TODO set output filesize limit
    int copy_file(const File &from, const File &to) {
      std::ifstream src(from);
      std::ofstream target(to);
      
      if (!src.is_open() || !target.is_open()) {
        // cannot open file
        std::cerr << "[warn] cannot open file " << from << " or " << to << std::endl;
        std::cerr << "\t" << strerror(errno) << std::endl;
        return 1;
      }
      
      target << src.rdbuf();
      
      std::cerr << "copy file from " << from << " to " << to << std::endl;
      
      src.close();
      target.close();
      
      return 0;
    }
}