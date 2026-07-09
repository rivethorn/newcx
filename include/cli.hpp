#pragma once

#include <ostream>
#include <string>

#include "ProjectGenerator.hpp"

class Cli
{
    ExeType exe_type_;
    ProjectType project_type_;
    std::string project_name_;

    void handle_exe_type(char *argv[]);
    void handle_newcx_project_type(std::string_view first_arg);
    void handle_project_type(std::string_view first_arg);
    void print_version(std::string_view exe);
    void print_usage_to(std::ostream &os, std::string_view exe);

public:
    Cli(int argc, char *argv[]);
    ProjectType project_type() const noexcept;
    std::string project_name() const noexcept;
};
