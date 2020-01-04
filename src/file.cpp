//
// Created by phirasit on 12/24/19.
//
#include "file.hpp"
#include "logger.hpp"

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
    
    static const Logger logger ("file-system");
    
    File base_dir(File path) {
      while (!path.empty() && path.back() == '/') path.pop_back();
      while (!path.empty() && path.back() != '/') path.pop_back();
      return path;
    }
    
    void change_folder(const File& path) {
      if (chdir(path.c_str())) {
        logger("[error] cannot change directory to ", path);
        logger("\t", strerror(errno));
      } else {
        logger("change directory to ", path);
      }
    }
    
    void create_folder(const File& path) {
      logger("creating new folder: ", path.c_str());
      if (mkdir(path.c_str(), 0770)) {
        if (errno != EEXIST) {
          logger("[error] cannot create a new folder: ", path, ": ", strerror(errno));
        } else {
          logger("[warning] file exists: ", path);
        }
      } else {
        logger("create new folder ", path);
      }
    }
    
    void remove_file(const File& path) {
      
      if (remove(path.c_str())) {
        logger("[error] cannot remove file ", path, ": ", strerror(errno));
      } else {
        logger(path, "is removed");
      }
    }
    
    void remove_folder(const File& path) {
      const std::string& command = "rm -rf " + path;
      if (system(command.c_str())) {
        logger("[error] cannot remove folder ", path, ": ", strerror(errno));
      } else {
        logger(path, " is removed");
      }
    }
    
    void create_symlink(const File& target, const File& link) {
      if (symlink(target.c_str(), link.c_str())) {
        logger("[error] cannot create symlink from ", link, " to ",  target, ": ", strerror(errno));
      } else {
        logger("create symlink from ", link, " to ", target);
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
      /*
      std::ifstream src(from);
      std::ofstream target(to);
      
      if (!src.is_open() || !target.is_open()) {
        // cannot open file
        logger("[warn] cannot open file ", from, " or ", to);
        logger("\t", strerror(errno));
        return 1;
      }
      
      target << src.rdbuf();
      
      logger("copy file from ", from, " to ", to);
      
      src.close();
      target.close();
       */
      logger("copy file from ", from, " to ", to);
      const std::string& command = "cp " + from + " " + to;
      return system(command.c_str());
    }
    
    int copy_folder(const File& from, const File& to) {
      create_folder(to);
      const std::string& command = std::string("cp -r ") + from + "/. " + std::string(to);
      logger("executing command: ", command);
      logger("copy folder from ", from, " to ", to);
      return system(command.c_str());
    }
}