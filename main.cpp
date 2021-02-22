#include "ft_irc.hpp"

int main(int argc, char **argv)
{
    IrcServer server(argc, argv);
    server.run(argc);
}