#include <filesystem>
#include <fstream>

#include "project_generator.hpp"

namespace fs = std::filesystem;

[[nodiscard]] Result<void> write_file(const fs::path &path,
                                      std::string_view contents)
{
    std::ofstream f(path, std::ios::out | std::ios::trunc);
    if (!f)
    {
        return std::unexpected(NewcxError::IoFailure);
    }
    f << contents;
    if (!f)
    {
        return std::unexpected(NewcxError::IoFailure);
    }
    return {};
}

[[nodiscard]] Result<void> make_dir(const fs::path &path)
{
    std::error_code ec;
    fs::create_directory(path, ec);
    if (ec)
    {
        return std::unexpected(NewcxError::IoFailure);
    }
    return {};
}
