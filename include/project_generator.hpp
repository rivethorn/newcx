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
    C90,
    C99,
    C11,
    C17,
    C23,
};
