#pragma once

#include <filesystem>
#include <string>

#include "ProjectGenerator.hpp"

namespace fs = std::filesystem;

class AppConfig
{
    fs::path config_path_ =
        fs::path(fs::path(std::getenv("HOME")) / ".newcx.toml");

    Result<void> parse_config();

public:
    C_Standard c_std;
    CXX_Standard cxx_std;

    AppConfig();
};

const std::string default_config = R"(c-standard = "11"
cpp-standard = "11"
)";
