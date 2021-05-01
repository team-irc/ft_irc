#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include <signal.h>

#define BUFFER_SIZE 512

int sock;

void error(std::string err)
{
    std::cout << err << std::endl;
    exit(1);
}

void connect_to_server(int &sock, int argc, char **argv)
{
	struct sockaddr_in serv_adr;

	if (argc != 3 && argc != 4)
		error("USAGE: ./chatbot <host> <port> <pwd>");
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		error("socket error");
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));

	if (connect(sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1)
		error("connect error");
}

void send_user_info(int &sock, int argc, char **argv)
{
	if (argc == 4)
	{
		std::string pass;
		pass = "pass " + std::string(argv[3]) + "\n";
    	write(sock, pass.c_str(), pass.length());
	}
    write(sock, "nick nonblock\n", 14);
    write(sock, "user n n n n\n", 13);
}

void run(int &sock)
{
	std::cout << "run" << std::endl;
	while (1)
    {
		std::string tmp = "privmsg bot :/help server\n"; // message for send ** newline always required **
		write(sock, tmp.c_str(), tmp.length());
    }
}

void sigint(int sig)
{
	close(sock);
	exit(0);
}

int main(int argc, char **argv)
{
	signal(SIGINT, sigint);
	connect_to_server(sock, argc, argv);
	send_user_info(sock, argc, argv);
    run(sock);
	close(sock);
	return (0);
}
