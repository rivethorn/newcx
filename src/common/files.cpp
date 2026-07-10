#include <filesystem>
#include <fstream>

#include "project_generator.hpp"

namespace fs = std::filesystem;

fs::path config_path()
{
#ifdef _WIN32
    if (const char *userprofile = std::getenv("USERPROFILE"))
    {
        return fs::path(userprofile) / ".newcx.toml";
    }

    if (const char *appdata = std::getenv("APPDATA"))
    {
        return fs::path(appdata) / "newcx" / "newcx.toml";
    }
#else
    if (const char *home = std::getenv("HOME"))
    {
        return fs::path(home) / ".newcx.toml";
    }
#endif

    return fs::path(".newcx.toml");
}

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
