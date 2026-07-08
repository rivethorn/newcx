#pragma once

#include <optional>

enum ProjectType
{
    C,
    CPP,
};

class CliArgs
{
    std::optional<ProjectType> type_;

public:
    explicit CliArgs(int, char *argv[]);

    constexpr void print_usage();

    std::optional<ProjectType> project_type() const noexcept;
};
