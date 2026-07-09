#include "cli.hpp"
#include "config.hpp"

int main(int argc, char *argv[])
{
    Cli cli(argc, argv);

    AppConfig conf = AppConfig();

    return 0;
}
