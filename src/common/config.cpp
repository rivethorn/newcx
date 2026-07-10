#include <cstdlib>
#include <expected>
#include <filesystem>
#include <fstream>
#include <ios>
#include <iostream>
#include <string>
#include <utility>

#include "config.hpp"
#include "files.hpp"
#include "project_generator.hpp"
#include "toml.hpp"

namespace fs = std::filesystem;

[[nodiscard]] AppConfig::AppConfig() : config_path_(config_path())
{
    if (auto parent = config_path_.parent_path(); !parent.empty())
    {
        fs::create_directories(parent);
    }

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
        this->c_std_ = std::move(static_cast<C_Standard>(c_std));
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
        this->cxx_std_ = static_cast<CXX_Standard>(cxx_std);
        break;
    default:
        return std::unexpected(NewcxError::InvalidConfig);
    }

    return {};
}

[[nodiscard]] std::string_view AppConfig::c_std() const noexcept
{
    switch (c_std_)
    {
    case C_Standard::C90:
        return "90";
    case C_Standard::C99:
        return "99";
    case C_Standard::C11:
        return "11";
    case C_Standard::C17:
        return "17";
    case C_Standard::C23:
        return "23";
    default:
        std::unreachable();
    }
}

[[nodiscard]] std::string_view AppConfig::cxx_std() const noexcept
{
    switch (cxx_std_)
    {
    case CXX_Standard::CXX98:
        return "98";
    case CXX_Standard::CXX11:
        return "11";
    case CXX_Standard::CXX14:
        return "14";
    case CXX_Standard::CXX17:
        return "17";
    case CXX_Standard::CXX20:
        return "20";
    case CXX_Standard::CXX23:
        return "23";
    case CXX_Standard::CXX26:
        return "26";
    default:
        std::unreachable();
    }
}
