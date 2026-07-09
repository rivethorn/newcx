#pragma once

#include <string_view>

inline constexpr std::string_view main_usage =
    R"(newcx - create a new c or cpp project, quickly
Usage:
    newcx -c <project-name>     Create a new C project
    newcx -cpp <project-name>   Create a new CPP project
    newcx -v, --version         Show app version)";
