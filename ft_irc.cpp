#include "ft_irc.hpp"

irc_server::irc_server(int argc, char **argv)
{
    if (argc != 2)
        std::cout << "error" << std::endl;
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1)
        std::cout << "error" << std::endl;
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1)
        std::cout << "error" << std::endl;
    
    if (listen(serv_sock, 5) == -1)
        std::cout << "error" << std::endl;

    client_adr_size = sizeof(client_adr);
};

void irc_server::run()
{
    char message[128];
    int str_len;

    for (int i = 0; i < 5; ++i)
    {
        client_sock = accept(serv_sock, (struct sockaddr *)&client_adr, &client_adr_size);
        if (client_sock == -1)
            std::cout << "error" << std::endl;
        else
            std::cout << "connect" << std::endl;
        
        while ((str_len = read(client_sock, message, 128)) != 0)
            write(client_sock, message, str_len);
        close(client_sock);
    }
    close(serv_sock);
};