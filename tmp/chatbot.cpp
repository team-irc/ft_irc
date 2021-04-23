#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>

int split(const std::string str, char c, std::string *& ret)
{
	int size;
	int str_counter;

	size = 0;
	str_counter = 0;
	for (int i = 0; i < str.length();)
	{
		if (str[i] == c)
		{
			++size;
			while (str[i] == c)
				++i;
		}
		else
			++i;
	}
	++size;
	ret = new std::string[size + 1];
	for (int i = 0; i < size; ++i)
	{
		for (; (str[str_counter] != c && str_counter < str.length()); ++str_counter)
			ret[i] += str[str_counter];
		while (str[str_counter] == c)
			++str_counter;
	}
	ret += 0;
	return (size);
}

// :yochoi!~y@121.135.181.42 PRIVMSG bot :hello world!!

void ignore_motd(int sock)
{
	char buf[512];
	while (1)
	{
		read(sock, buf, 512);
		std::string *ret;
		split(std::string(buf), ' ', ret);
		if (ret[3] == ":End")
		{
			delete[] ret;
			break ;
		}
		delete[] ret;
	}
}

std::string get_message_type(char *message)
{
	std::string	ret;
	int			i = (message[0] == ':' ? 1 : 0);

	while (!std::isupper(message[i]) && message[i] && message[i] != ':')
		++i;
	if (message[i] == ':')
		return (std::string());
	while (message[i] != ' ')
		ret += message[i++];
	return (ret);
}

std::string get_command(char *message)
{
	std::string	tmp(message);
	std::string	ret;
	std::string::reverse_iterator first;
	std::string::reverse_iterator last;

	first = tmp.rbegin();
	last = tmp.rend();
	while (first != last && *first != ' ')
	{
		ret += *first;
		++first;
	}
	std::reverse(ret.begin(), ret.end());
	if (*(ret.begin()) == ':')
		ret.erase(ret.begin());
	while (*(ret.end() - 1) == '\n' || *(ret.end() - 1) == '\r')
		ret.erase(ret.end() - 1);
	return (ret);
}

std::string get_target(char *message)
{
	std::string	ret;
	int			i = 0;

	while (message[i] != '!')
		ret += message[i++];
	if (*(ret.begin()) == ':')
		ret.erase(ret.begin());
	return (ret);
}

void	hello_command(int sock, std::string target)
{
	std::string ret;

	ret += "privmsg " + target + " :hello from bot\n";
	std::cout << "[SEND] " << ret;
	write(sock, ret.c_str(), ret.length());
}

void	unknown_command(int sock, std::string target, std::string command)
{
	std::string ret;

	ret = "privmsg " + target + " :Unknown Command: " + command + '\n';
	std::cout << "[SEND] " << ret;
	write(sock, ret.c_str(), ret.length());
}

void error(std::string err)
{
    std::cout << err << std::endl;
    exit(1);
}

int main(int argc, char **argv)
{
	int sock;
	char message[512];
	int str_len;
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
	std::cout << "connect" << std::endl;
	if (argc == 4)
	{
		std::string pass;
		std::string arg(argv[3]);
		pass = "pass " + arg + "\n";
    	write(sock, pass.c_str(), pass.length());
	}
    write(sock, "nick bot\n", 9);
    write(sock, "user b b b b\n", 13);
	ignore_motd(sock);
	std::cout << "ignore complete\n";
    while (1)
    {
		memset(message, 0, 512);
        read(sock, message, 512);
		std::cout << "[RECV] " << message;
		std::string message_type = get_message_type(message); // PRIVMSG
		if (message_type == "PRIVMSG")
		{
			std::string command = get_command(message); // /help
			std::string target = get_target(message); // yochoi	
			if (command == "/hello")
				hello_command(sock, target);
			else
				unknown_command(sock, target, command);
		}
		if (message_type == "PING")
			write(sock, "notice bot :i am alive\n", 23);
    }
	close(sock);
	return (0);
}