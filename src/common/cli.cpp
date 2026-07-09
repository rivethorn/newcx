#include <filesystem>
#include <iostream>
#include <string_view>

#include "cli.hpp"
#include "project_generator.hpp"

namespace fs = std::filesystem;

Cli::Cli(int argc, char *argv[])
{
    handle_exe_type(argv);

    if (exe_type_ == ExeType::newcx && (argc == 1 || argc > 3))
    {
        print_usage_to(std::cerr, to_string(exe_type_));
        std::exit(1);
    }

    if (exe_type_ != ExeType::newcx && (argc == 1 || argc > 2))
    {
        print_usage_to(std::cerr, to_string(exe_type_));
        std::exit(1);
    }

    const std::string_view first_arg{argv[1]};

    if (first_arg == "-v" || first_arg == "--version")
    {
        print_version(to_string(exe_type_));
        std::exit(0);
    }

    handle_project_type(first_arg);

    switch (exe_type_)
    {
    case ExeType::newc:
    case ExeType::newcpp:
        project_name_ = argv[1];
        break;
    case ExeType::newcx:
        if (argc == 2)
        {
            print_usage_to(std::cerr, to_string(exe_type_));
            std::exit(1);
        }
        project_name_ = argv[2];
        break;
    }
}

[[nodiscard]] ProjectType Cli::project_type() const noexcept
{
    return project_type_;
}

[[nodiscard]] std::string Cli::project_name() const noexcept
{
    return project_name_;
}

void Cli::handle_exe_type(char *argv[])
{
    fs::path exe = argv[0];
    auto name = exe.filename().string();

    if (name == "newc")
    {
        exe_type_ = ExeType::newc;
    }
    else if (name == "newcpp")
    {
        exe_type_ = ExeType::newcpp;
    }
    else
    {
        exe_type_ = ExeType::newcx;
    }
}

void Cli::handle_newcx_project_type(std::string_view first_arg)
{
    if (first_arg == "-c")
    {
        project_type_ = ProjectType::C;
    }
    else if (first_arg == "-cpp")
    {
        project_type_ = ProjectType::CPP;
    }
    else
    {
        print_usage_to(std::cerr, to_string(exe_type_));
        std::exit(1);
    }
}

void Cli::handle_project_type(std::string_view first_arg)
{
    switch (exe_type_)
    {
    case ExeType::newc:
        project_type_ = ProjectType::C;
        break;
    case ExeType::newcpp:
        project_type_ = ProjectType::CPP;
        break;
    case ExeType::newcx:
        handle_newcx_project_type(first_arg);
        break;
    }
}

void Cli::print_usage_to(std::ostream &os, std::string_view exe)
{
    if (exe == "newcx")
    {
        std::println(
            os, "newcx - create a new C/CPP project, quickly\n"
                "Usage:\n"
                "    newcx -c <project-name>     Create a new C project\n"
                "    newcx -cpp <project-name>   Create a new CPP project\n"
                "    newcx -v, --version         Show app version");
    }
    else if (exe == "newc")
    {
        std::println(os,
                     "newc - create a new C project, quickly\n"
                     "Usage:\n"
                     "    newc <project-name>        Create a new C project\n"
                     "    newc -v, --version         Show app version");
    }
    else if (exe == "newcpp")
    {
        std::println(
            os, "newcpp - create a new CPP project, quickly\n"
                "Usage:\n"
                "    newcpp <project-name>        Create a new CPP project\n"
                "    newcpp -v, --version         Show app version");
    }
}

void Cli::print_version(std::string_view exe)
{
    std::println("{} - scaffold a new C/CPP project, quickly\n"
                 "version {}",
                 exe, APP_VERSION);
}
