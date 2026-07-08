#pragma once

#include <optional>

template <typename T> using Option = std::optional<T>;

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
