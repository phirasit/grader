# Grader
This grader is built for generic code testing system. 
The main focus of this project is to build a grader worker that is robust
and supports most if not all types of code testing. It is hope that this project will be
integrated to many available grading system and later on help to standardize how
code test file strucutre should look.

## Build
1. clone this project `git clone --recursive https://github.com/phirasit/grader`.
2. run cmake `cmake .` on the root directory.
3. run make `make` to build the project.  
**Note** the build result will be stored in `bin/` directory

## Install
1. run `bin/setup`. If root permission is needed, run `sudo bin/setup` instead.  
**[OPTIONAL]** 
move all the executables in `bin/` to `/usr/local/bin` to make the program
accessible from anywhere
`cp bin/* /usr/local/bin`.

## Run
This is a list of programs that can be run

| Program         | Description       |
|-----------------|-------------------|
| `setup`         | setup script |
| `sandbox`       | run sandbox program that containerizes a program |
| `task`          | manage all tasks in the system directly |
| `grader-server` | start grader rest-api server |

In default, all the data of the grader will be kept at `/grader` folder.
This can be changed by setting `GRADER_BASE_DIR` environment variable before
running any program (i.e. `GRADER_BASE_DIR=~/.grader bin/setup`).

How to use each program can be seen in help section of the program.
