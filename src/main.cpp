#include "cli.hpp"

#include <print>

int main(int argc, char *argv[])
{
    CliArgs cli(argc, argv);

    if (cli.project_type() == ProjectType::C)
    {
        std::println("C");
    }
    else if (cli.project_type() == ProjectType::CPP)
    {
        std::println("CPP");
    }
    else
    {
        std::println("no type");
    }

    return 0;
}
