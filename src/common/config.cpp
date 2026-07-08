#include <filesystem>
#include <fstream>
#include <ios>
#include <iostream>
#include <string_view>
#include <utility>

#include "config.hpp"
#include "project_generator.hpp"
#include "toml.hpp"

namespace fs = std::filesystem;

AppConfig::AppConfig()
{
    if (!fs::exists(config_path_))
    {
        std::ofstream f(config_path_, std::ios::out);
        // TODO: Handle Error
        f << default_config;
    }

    parse_config();

    std::println("{}", static_cast<int>(c_std));
    std::println("{}", static_cast<int>(cxx_std));
}

void AppConfig::parse_config()
{
    auto config = toml::parse_file(config_path_.string());

    auto c_std = std::stoi(config["c-standard"].value_or("11"));
    auto cxx_std = std::stoi(config["cpp-standard"].value_or("11"));

    switch (c_std)
    {
    case 90:
    case 99:
    case 11:
    case 17:
    case 23:
        this->c_std = std::move(static_cast<C_Standard>(c_std));
        break;
    }

    switch (cxx_std)
    {
    case 98:
    case 11:
    case 14:
    case 17:
    case 20:
    case 23:
    case 26:
        this->cxx_std = static_cast<CXX_Standard>(cxx_std);
        break;
    }
}
