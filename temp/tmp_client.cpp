#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void error()
{
    std::cout << "error" << std::endl;
    exit(1);
}

int main(int argc, char **argv)
{
	int sock;
	char message[128];
	int str_len;
	struct sockaddr_in serv_adr;

	if (argc != 3)
		error();
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		error();
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));

	if (connect(sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1)
		error();
	else
		std::cout << "connected" << std::endl;

	while (1)
	{
		fputs("input: ", stdout);
		fgets(message, 128, stdin);

		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break;

		write(sock, message, strlen(message));
		str_len = read(sock, message, 128 - 1);
		message[str_len] = 0;
		std::cout << message << std::endl;
	}
	close(sock);
	return (0);
}