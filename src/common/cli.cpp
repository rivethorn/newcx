#include "cli.hpp"

#include <filesystem>
#include <print>

namespace fs = std::filesystem;

CliArgs::CliArgs(int, char *argv[])
{
    fs::path exe = argv[0];
    auto name = exe.filename().string();

    if (name == "newc")
    {
        type_ = ProjectType::C;
    }
    else if (name == "newcpp")
    {
        type_ = ProjectType::CPP;
    }
    else
    {
        type_ = std::nullopt;
    }
}

Option<ProjectType> CliArgs::project_type() const noexcept
{
    return type_;
}

constexpr void CliArgs::print_usage()
{
    std::println("usage");
}
