//
// Created by phirasit on 12/24/19.
//

#ifndef GRADER_FILE_HPP
#define GRADER_FILE_HPP

#include <experimental/filesystem>
#include <string>

namespace file {
    
    using File = std::string;
    
    File base_dir(File path);
    
    void change_folder(const File& path);
    void create_folder(const File& path);
    void remove_file(const File& path);
    void remove_folder(const File& path);
    void create_symlink(const File& link, const File& target);
    
    bool file_readable(const File& path);
    
    File get_path(const File& path);
    
    int copy_file(const File& src, const File& target);
    int copy_folder(const File& src, const File& target);
}

#endif //GRADER_FILE_HPP
