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
        std::println("C");
        std::println("{}", name);
    }
    else if (cli.project_type() == ProjectType::CPP)
    {
        std::println("CPP");
        std::println("{}", name);
    }

    return 0;
}
