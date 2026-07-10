#pragma once

#include <filesystem>
#include <string>

#include "project_generator.hpp"

namespace fs = std::filesystem;

class AppConfig
{
    C_Standard c_std_;
    CXX_Standard cxx_std_;
    fs::path config_path_;

    Result<void> parse_config();

public:
    AppConfig();
    std::string_view c_std() const noexcept;
    std::string_view cxx_std() const noexcept;
};

const std::string default_config = R"(c-standard = "11"
cpp-standard = "11"
)";
