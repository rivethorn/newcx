#pragma once

#include <filesystem>

#include "project_generator.hpp"

Result<void> write_file(const std::filesystem::path &, std::string_view);
Result<void> make_dir(const std::filesystem::path &);
