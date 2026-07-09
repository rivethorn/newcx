#pragma once

#include "project_generator.hpp"

class Cli
{
    ProjectType project_type_;
    ExeType exe_type_;

    void handle_main_exe(int argc, char *argv[]);

    constexpr void print_version(std::string_view);

public:
    explicit Cli(int, char *argv[]);

    ProjectType project_type() const noexcept;
};
