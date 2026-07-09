#include <expected>
#include <filesystem>
#include <fstream>
#include <ios>
#include <iostream>
#include <string>
#include <utility>

#include "config.hpp"
#include "toml.hpp"

namespace fs = std::filesystem;

[[nodiscard]] AppConfig::AppConfig()
{
    if (!fs::exists(config_path_))
    {
        std::ofstream f(config_path_, std::ios::out);
        if (!f)
        {
            std::println(std::cerr, "{}", to_string(NewcxError::IoFailure));
            std::exit(1);
        }
        f << default_config;
        f.close();
    }

    if (auto r = parse_config(); !r)
    {
        std::println(std::cerr, "{}", to_string(r.error()));
        std::exit(1);
    }
}

[[nodiscard]] Result<void> AppConfig::parse_config()
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
    default:
        return std::unexpected(NewcxError::InvalidConfig);
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
    default:
        return std::unexpected(NewcxError::InvalidConfig);
    }

    return {};
}
