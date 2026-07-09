#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <print>
#include <string_view>

#include "cli.hpp"

namespace fs = std::filesystem;

static void create_links(std::string_view exe)
{
    fs::path self = fs::canonical(exe);

    fs::path newc = self.parent_path() / "newc";
    fs::path newcpp = self.parent_path() / "newcpp";

    if (!fs::exists(newc))
    {
        fs::create_symlink(self.filename(), newc);
    }

    if (!fs::exists(newcpp))
    {
        fs::create_symlink(self.filename(), newcpp);
    }

    std::println("You can now use newc and newcpp");
}

static void handle_main_exe_call(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::println(
            std::cerr,
            "Please invoke this programs as either:\n\tnewc\n\tnewcpp\n\nYou "
            "can run the link command to create the symlinks");
        std::exit(1);
    }

    std::string_view exe = argv[0];
    std::string_view cmd = argv[1];

    if (cmd == "link")
    {
        create_links(exe);
        std::exit(0);
    }
    else
    {
        std::println(
            std::cerr,
            "Please invoke this programs as either:\n\tnewc\n\tnewcpp\n\nYou "
            "can run the link command to create the symlinks");
        std::exit(1);
    }
}

Cli::Cli(int argc, char *argv[])
{
    handle_exe_type(argv);

    if (exe_type_ == ExeType::newcx)
    {
        handle_main_exe_call(argc, argv);
    }

    if (argc == 1 || argc > 2)
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

    if (first_arg == "--edit-config")
    {
        auto editor = std::getenv("EDITOR");
        if (!editor)
        {
            editor = std::getenv("VISUAL");
        }
        if (!editor)
        {
            std::println("No default editor found.\nMake sure to set $EDITOR");
            std::exit(1);
        }

        fs::path config_path =
            fs::path(fs::path(std::getenv("HOME")) / ".newcx.toml");

        auto command =
            std::string(editor) + " \"" + config_path.string() + "\"";

        auto return_code = std::system(command.c_str());

        std::exit(return_code);
    }

    handle_project_type();

    switch (exe_type_)
    {
    case ExeType::newc:
    case ExeType::newcpp:
        project_name_ = argv[1];
        break;
    case ExeType::newcx:
        std::unreachable();
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

void Cli::handle_project_type()
{
    switch (exe_type_)
    {
    case ExeType::newc:
        project_type_ = ProjectType::C;
        break;
    case ExeType::newcpp:
        project_type_ = ProjectType::CXX;
        break;
    case ExeType::newcx:
        std::unreachable();
    }
}

void Cli::print_usage_to(std::ostream &os, std::string_view exe)
{
    if (exe == "newc")
    {
        std::println(
            os,
            "newc - create a new C project, quickly\n"
            "Usage:\n"
            "    newc <project-name>       Create a new C project\n"
            "    newc --edit-config        Open the config file in $EDITOR\n"
            "    newc -v, --version        Show app version");
    }
    else if (exe == "newcpp")
    {
        std::println(
            os,
            "newcpp - create a new CPP project, quickly\n"
            "Usage:\n"
            "    newcpp <project-name>      Create a new CPP project\n"
            "    newcpp --edit-config       Open the config file in $EDITOR\n"
            "    newcpp -v, --version       Show app version");
    }
}

void Cli::print_version(std::string_view exe)
{
    std::println("{} - create a new C/CPP project, quickly\n"
                 "version {}",
                 exe, APP_VERSION);
}
