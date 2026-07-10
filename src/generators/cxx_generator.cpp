#include <array>
#include <filesystem>
#include <format>
#include <print>
#include <string>

#include "files.hpp"
#include "project_generator.hpp"

namespace CXX_Generator
{

namespace fs = std::filesystem;

[[nodiscard]] std::string cmake_lists_txt(std::string_view project_name,
                                          std::string_view std_version)
{
    return std::format(
        "cmake_minimum_required(VERSION 3.20)\n"
        "project({0} CXX)\n"
        "\n"
        "set(CMAKE_CXX_STANDARD {1})\n"
        "set(CMAKE_CXX_STANDARD_REQUIRED ON)\n"
        "set(CMAKE_EXPORT_COMPILE_COMMANDS ON)\n"
        "\n"
        "if(NOT CMAKE_BUILD_TYPE)\n"
        "    set(CMAKE_BUILD_TYPE Debug)\n"
        "endif()\n"
        "\n"
        "set(CMAKE_CXX_FLAGS_DEBUG \"-g -O0 -Wall -Wextra "
        "-fsanitize=address,undefined\")\n"
        "set(CMAKE_EXE_LINKER_FLAGS_DEBUG \"-fsanitize=address,undefined\")\n"
        "set(CMAKE_CXX_FLAGS_RELEASE \"-O2\")\n"
        "\n"
        "file(GLOB SOURCES \"${{CMAKE_SOURCE_DIR}}/src/*.cpp\")\n"
        "\n"
        "add_executable({0} ${{SOURCES}})\n"
        "\n"
        "target_include_directories({0} PRIVATE "
        "${{CMAKE_SOURCE_DIR}}/include)\n",
        project_name, std_version);
}

[[nodiscard]] std::string makefile(std::string_view project_name,
                                   std::string_view std_version)
{
    return std::format(
        "CC       := g++\n"
        "CFLAGS   := -std=c++{1} -Wall -Wextra\n"
        "LDFLAGS  :=\n\n"
        "BUILD_TYPE ?= debug\n\n"
        "ifeq ($(BUILD_TYPE),debug)\n"
        "\tCFLAGS  += -g -O0 -fsanitize=address,undefined\n"
        "\tLDFLAGS += -fsanitize=address,undefined\n"
        "else\n"
        "\tCFLAGS  += -O2\n"
        "endif\n\n"
        "SRC_DIR     := src\n"
        "INCLUDE_DIR := include\n"
        "OUT_DIR     := out\n\n"
        "SOURCES := $(wildcard $(SRC_DIR)/*.cpp)\n"
        "TARGET  := $(OUT_DIR)/{0}\n\n"
        ".PHONY: all run clean\n\n"
        "all: $(TARGET)\n\n"
        "$(TARGET): $(SOURCES)\n"
        "\t@mkdir -p $(OUT_DIR)\n"
        "\t$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -o $(TARGET) $(SOURCES) "
        "$(LDFLAGS)\n\n"
        "run: all\n"
        "\t./$(TARGET)\n\n"
        "clean:\n"
        "\trm -rf $(OUT_DIR)",
        project_name, std_version);
}

[[nodiscard]] std::string main_cpp(std::string_view project_name)
{
    return std::format("#include <iostream>\n"
                       "#include \"{0}.h\"\n"
                       "\n"
                       "int main() {{\n"
                       "    std::cout << \"{0} initialized.\\n\";\n"
                       "    return 0;\n"
                       "}}\n",
                       project_name);
}

[[nodiscard]] std::string header_hpp(std::string_view project_name)
{
    return std::format("#pragma once\n"
                       "\n"
                       "/* {} public interface */\n"
                       "\n",
                       project_name);
}

[[nodiscard]] std::string gitignore()
{
#if defined(__APPLE__) || defined(__MACH__)
    return "out/\ncompile_commands.json\n.DS_Store*.o\n";
#else
    return "out/\ncompile_commands.json\n*.o\n";
#endif
}

[[nodiscard]] Result<void> scaffold_project(std::string_view project_name,
                                            std::string_view std_version)
{
    if (project_name.empty())
    {
        return std::unexpected(NewcxError::InvalidName);
    }
    if (project_name.size() >= MAX_NAME_LEN)
    {
        return std::unexpected(NewcxError::NameTooLong);
    }

    const fs::path root{project_name};
    if (fs::exists(root))
    {
        return std::unexpected(NewcxError::AlreadyExists);
    }

    const std::array<fs::path, 4> dirs{root, root / "src", root / "include",
                                       root / "out"};
    for (const fs::path &dir : dirs)
    {
        if (auto r = make_dir(dir); !r)
        {
            return r;
        }
    }

    const std::string header_filename = std::format("{}.h", project_name);

    struct FileSpec
    {
        fs::path path;
        std::string contents;
    };

    const std::array<FileSpec, 5> files{{
        {root / "CMakeLists.txt", cmake_lists_txt(project_name, std_version)},
        {root / "Makefile", makefile(project_name, std_version)},
        {root / "src" / "main.cpp", main_cpp(project_name)},
        {root / "include" / header_filename, header_hpp(project_name)},
        {root / ".gitignore", gitignore()},
    }};

    for (const auto &[path, contents] : files)
    {
        if (auto r = write_file(path, contents); !r)
        {
            return r;
        }
    }

    std::println("Created '{}':", project_name);
    std::println("  {}/CMakeLists.txt", project_name);
    std::println("  {}/Makefile", project_name);
    std::println("  {}/src/main.cpp", project_name);
    std::println("  {}/include/{}", project_name, header_filename);
    std::println("  {}/.gitignore", project_name);
    std::println("  {}/out/  (output directory)", project_name);

    return {};
}
} // namespace CXX_Generator
