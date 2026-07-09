#include <print>

#include "cli.hpp"
#include "config.hpp"
#include "project_generator.hpp"

int main(int argc, char *argv[])
{
    Cli cli(argc, argv);
    auto name = cli.project_name();

    AppConfig conf = AppConfig();

    if (cli.project_type() == ProjectType::C)
    {
        auto result = C_Generator::scaffold_project(name, conf.c_std());
        if (!result)
        {
            std::println(stderr, "Error: {}: {}", to_string(result.error()),
                         name);
            return 1;
        }
    }
    else if (cli.project_type() == ProjectType::CXX)
    {
        auto result = CXX_Generator::scaffold_project(name, conf.cxx_std());
        if (!result)
        {
            std::println(stderr, "Error: {}: {}", to_string(result.error()),
                         name);
            return 1;
        }
    }

    return 0;
}
