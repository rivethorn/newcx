#include <filesystem>
#include <iostream>
#include <span>
#include <string_view>

#include "cli.hpp"
#include "messages.hpp"
#include "project_generator.hpp"

namespace fs = std::filesystem;

// Initialize the app and check which executable was called.
Cli::Cli(int argc, char *argv[])
{
    fs::path exe = argv[0];
    auto name = exe.filename().string();

    if (name == "newc")
    {
        exe_type_ = ExeType::newc;
        project_type_ = ProjectType::C;
    }
    else if (name == "newcpp")
    {
        exe_type_ = ExeType::newcpp;
        project_type_ = ProjectType::CPP;
    }
    else
    {
        exe_type_ = ExeType::newcx;
        handle_main_exe(argc, argv);
    }
}

ProjectType Cli::project_type() const noexcept
{
    return project_type_;
}

constexpr void Cli::print_version(std::string_view exe)
{
    std::println("{} - scaffold a new C project, quickly\n"
                 "version {}",
                 exe, APP_VERSION);
}

void Cli::handle_main_exe(int argc, char *argv[])
{
    const std::span<char *> args{argv, static_cast<std::size_t>(argc)};

    if (argc < 2 || argc > 3)
    {
        std::println(std::cerr, "{}", main_usage);
        std::exit(1);
    }

    const std::string_view first_arg{args[1]};

    if (first_arg == "-v" || first_arg == "--version")
    {
        print_version(to_string(exe_type_));
        std::exit(0);
    }

    if (first_arg == "-c")
    {
        project_type_ = ProjectType::C;
    }
    else if (first_arg == "-cpp")
    {
        project_type_ = ProjectType::CPP;
    }
}
