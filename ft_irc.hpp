#ifndef FT_IRC_HPP
#define FT_IRC_HPP

#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

class irc_server
{
private:
    int serv_sock;
    int client_sock;
    struct sockaddr_in serv_adr;
    struct sockaddr_in client_adr;
    socklen_t client_adr_size;
public:
    irc_server(int argc, char **argv);
public:
    void run();
};

#endif