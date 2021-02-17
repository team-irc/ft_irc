#include "ft_irc.hpp"

int main(int argc, char **argv)
{
    irc_server server(argc, argv);
    server.run();
}