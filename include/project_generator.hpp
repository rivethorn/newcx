#pragma once

#include <expected>
#include <optional>
#include <string_view>

enum class NewcxError
{
    InvalidConfig,
    IoFailure,
};

[[nodiscard]] constexpr std::string_view to_string(NewcxError err)
{
    switch (err)
    {
    case NewcxError::InvalidConfig:
        return "invalid configuration file";
    case NewcxError::IoFailure:
        return "an I/O error occurred";
    }
    return "unknown error";
}

template <typename T> using Option = std::optional<T>;
template <typename T> using Result = std::expected<T, NewcxError>;

enum class ProjectType
{
    C,
    CPP,
};

enum class C_Standard
{
    C90 = 90,
    C99 = 99,
    C11 = 11,
    C17 = 17,
    C23 = 23,
};

enum class CXX_Standard
{
    CXX98 = 98,
    CXX11 = 11,
    CXX14 = 14,
    CXX17 = 17,
    CXX20 = 20,
    CXX23 = 23,
    CXX26 = 26,
};
