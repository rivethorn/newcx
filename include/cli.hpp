#pragma once

#include "project_generator.hpp"

class CliArgs
{
    Option<ProjectType> type_;

public:
    explicit CliArgs(int, char *argv[]);

    constexpr void print_usage();

    Option<ProjectType> project_type() const noexcept;
};
