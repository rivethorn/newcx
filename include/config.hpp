#pragma once

#include <cstdlib>
#include <filesystem>

#include "project_generator.hpp"

namespace fs = std::filesystem;

class AppConfig
{
    fs::path config_path_ =
        fs::path(fs::path(std::getenv("HOME")) / ".newcx.toml");

    void parse_config();

public:
    C_Standard c_std;
    CXX_Standard cxx_std;

    AppConfig();
};

const std::string default_config = R"(c-standard = "11"
cpp-standard = "11"
)";
