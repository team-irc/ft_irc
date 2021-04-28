#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include <signal.h>
#include <fcntl.h>

#define BUFFER_SIZE 512

int sock;

void error(std::string err)
{
    std::cout << err << std::endl;
    exit(1);
}

static std::string		remember_to_buf(std::string &remember)
{
	std::string		result;
	
	for (int i = 0; i < remember.length(); i++)
	{
		if (remember.at(i) != '\r' && remember.at(i) != '\n')
			result += remember.at(i);
		else
		{
			result += '\n';
			if (remember.at(i) == '\r' &&
				(i + 1) < remember.length() && remember.at(i + 1) == '\n')
				i++;
			remember = remember.substr(i + 1);
			return (result);
		}
	}
	remember.clear();
	return (result);
}

int	read_until_crlf(int fd, char *buffer, int *len)
{
	int					i = 0;
	int					read_size = 0;
	int					insert_idx = 0;
	char				buf[BUFFER_SIZE + 1];
	static std::string	remember[OPEN_MAX];
	int					rem_size = 0;

	memset(buf, 0, BUFFER_SIZE);
	if (!remember[fd].empty())
	{
		std::string	result = remember_to_buf(remember[fd]);
		rem_size = result.length();
		strncpy(buf, result.c_str(), rem_size);
		insert_idx = rem_size;
	}
	while (insert_idx < BUFFER_SIZE)
	{
		if (remember[fd].empty())
		{
			if ((read_size = read(fd, buf + insert_idx, BUFFER_SIZE - insert_idx)) == -1)
				break ;
			else if (read_size == 0)
				exit(0);
			buf[insert_idx + read_size] = 0;
		}
		else
		{
			if (insert_idx >= 1 && buf[insert_idx - 1] != '\n')
			{
				std::string	result = remember_to_buf(remember[fd]);
				rem_size = result.length();
				strncpy(buf + insert_idx, result.c_str(), rem_size);
				insert_idx += rem_size;
			}
		}
		for (i = 0; i < read_size + rem_size; i++)
		{
			if (buf[i] == '\r' || buf[i] == '\n')
			{
				if (rem_size == 0)
				{
					strncpy(buffer, buf, i + 1);
					buffer[i + insert_idx + 1] = 0;
					if (buffer[i] == '\r')
						buffer[i] = '\n';
				}
				else
				{
					strncpy(buffer, buf, i + 1);
					buffer[i + 1] = 0;
					if (buffer[i] == '\r')
						buffer[i] = '\n';
				}
				if (buf[i + 1] == '\n')
					++i;
				for (int j = 1; buf[i + j]; ++j)
					remember[fd] += buf[i + j];
				*len = i + insert_idx;
				if (remember[fd].empty())
					return (0);
				return (1);
			}
		}
		rem_size = 0;
		remember[fd] += buf;
		return (2);
	}
	for (int i = 0; buf[i] != 0; i++)
	{
		buffer[i] = buf[i];
	}
	buffer[insert_idx] = 0;
	*len = insert_idx;
	if (remember[fd].empty())
		return (0);
	else
		return (1);
}

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

std::string get_command(char *message)
{
	std::string	ret;
	std::string *split_ret;

	split(message, ' ', split_ret);
	ret = split_ret[3];
	delete[] split_ret;
	if (ret[0] == ':')
		ret.erase(ret.begin());
	if (ret[ret.length() - 1] == '\n' || ret[ret.length() - 1] == '\r')
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

void	help_command(int sock, std::string target, std::string message)
{
	const std::string privmsg = "privmsg " + target + ":";
	if (*(message.begin()) == ':')
		message.erase(message.begin()); // erase first colon
	if (message.find(':') != std::string::npos)
	{
		std::string tmp;
		std::string *split_ret;
		split(message, ':', split_ret);
		std::string command = split_ret[1];
		delete[] split_ret;
		if (split(command, ' ', split_ret) == 1)
		{
			tmp = privmsg + "USAGE: privmsg bot :/help <command>\n";
			write(sock, tmp.c_str(), tmp.length());
			return ;
		}
		else
		{
			command = split_ret[1];
			delete[] split_ret;
			if (command[command.length() - 1] == '\n' || command[command.length() - 1] == '\r')
				command.erase(command.end() - 1);
			std::transform(command.begin(), command.end(), command.begin(), ::toupper);
			if (command == "ADMIN")
				tmp = privmsg + "USAGE: ADMIN [<server>]\n";
			else if (command == "AWAY")
				tmp = privmsg + "USAGE: AWAY [message]\n";
			else if (command == "CONNECT")
				tmp = privmsg + "USAGE: CONNECT <target server> [<port> [<remote server>]]\n";
			else if (command == "ERROR")
				tmp = privmsg + "USAGE: ERROR <error message>\n";
			else if (command == "INFO")
				tmp = privmsg + "USAGE: INFO [<server>]\n";
			else if (command == "INVITE")
				tmp = privmsg + "USAGE: INVITE <nickname> <channel>\n";
			else if (command == "ISON")
				tmp = privmsg + "USAGE: ISON <nickname>{<space><nickanem>}\n";
			else if (command == "JOIN")
				tmp = privmsg + "USAGE: JOIN <channel>{,<channel>} [<key>,{<key>}]\n";
			else if (command == "KICK")
				tmp = privmsg + "USAGE: KICK <channel> <user> [<comment>]\n";
			else if (command == "KILL")
				tmp = privmsg + "USAGE: KILL <nickname> <comment>\n";
			else if (command == "LINKS")
				tmp = privmsg + "USAGE: LINKS [[<remote server>] <server mask>]\n";
			else if (command == "LIST")
				tmp = privmsg + "USAGE: LIST [<channel>,{<channel>} [<server]\n";
			else if (command == "MODE")
				tmp = privmsg + "USAGE: MODE <channel> {[+|-]|o|p|s|i|t|n|b|v} [<limit>] [<user>] [<ban mask>] OR MODE <nickname> {[+|-]|i|w|s|o}\n";
			else if (command == "NAMES")
				tmp = privmsg + "USAGE: NAMES [<channel>{,<channel>}]\n";
			else if (command == "NICK")
				tmp = privmsg + "USAGE: NICK <nickname> [ <hopcount> ]\n";
			else if (command == "NOTICE")
				tmp = privmsg + "USAGE: NOTICE <nickname> <text>\n";
			else if (command == "OPER")
				tmp = privmsg + "USAGE: OPER <user> <password>\n";
			else if (command == "PART")
				tmp = privmsg + "USAGE: PART <channel>{,<channel>}\n";
			else if (command == "PASS")
				tmp = privmsg + "USAGE: PASS <password>\n";
			else if (command == "PING")
				tmp = privmsg + "USAGE: PING <server1> [<server2>]\n";
			else if (command == "PONG")
				tmp = privmsg + "USAGE: PONG <daemon> [<daemon2>]\n";
			else if (command == "PRIVMSG")
				tmp = privmsg + "USAGE: PRIVMSG <receiver>{,<receiver>} <text to be sent>\n";
			else if (command == "QUIT")
				tmp = privmsg + "USAGE: QUIT [<Quit message>]\n";
			else if (command == "REHASH")
				tmp = privmsg + "USAGE: REHASH\n";
			else if (command == "SERVER")
				tmp = privmsg + "USAGE: SERVER <servername> <hopcount> <info>\n";
			else if (command == "SQUIT")
				tmp = privmsg + "USAGE: SQUIT <server> <comment>\n";
			else if (command == "STATS")
				tmp = privmsg + "USAGE: STATS [<query> [<server>]]\n";
			else if (command == "TIME")
				tmp = privmsg + "USAGE: TIME [<server>]\n";
			else if (command == "TOPIC")
				tmp = privmsg + "USAGE: TOPIC <channel> [<topic>]\n";
			else if (command == "TRACE")
				tmp = privmsg + "USAGE: TRACE [<server>]\n";
			else if (command == "USER")
				tmp = privmsg + "USAGE: USER <username> <hostname> <servername> <realname>\n";
			else if (command == "USERHOST")
				tmp = privmsg + "USAGE: USERHOST <nickname>{<space><nickname>}\n";
			else if (command == "VERSION")
				tmp = privmsg + "USAGE: VERSION [<server>]\n";
			else if (command == "WHOIS")
				tmp = privmsg + "USAGE: WHOIS [<server>] <nickmask>[,<nickmask>[,...]]\n";
			else if (command == "WHO")
				tmp = privmsg + "USAGE: WHO [<name> [<o>]]\n";
			else if (command == "WHOWAS")
				tmp = privmsg + "USAGE: WHOWAS <nickname> [<count> [<server>]]\n";
			else
				tmp = privmsg + "unknown command: " + command + '\n';
		}
		write(sock, tmp.c_str(), tmp.length());
	}
	else
	{
		std::string tmp = privmsg + "USAGE: privmsg bot :/help <command>\n";
		write(sock, tmp.c_str(), tmp.length());
	}
}

void	unknown_command(int sock, std::string target, std::string command)
{
	std::string ret;

	ret = "privmsg " + target + " :Unknown Command: " + command + '\n';
	std::cout << "[SEND] " << ret;
	write(sock, ret.c_str(), ret.length());
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
	fcntl(sock, F_SETFL, O_NONBLOCK);
}

void send_user_info(int &sock, int argc, char **argv)
{
	if (argc == 4)
	{
		std::string pass;
		pass = "pass " + std::string(argv[3]) + "\n";
    	write(sock, pass.c_str(), pass.length());
	}
    write(sock, "nick bot\n", 9);
    write(sock, "user b b b b\n", 13);
}

void run(int &sock)
{
	char message[512];
	std::string	*split_ret;
	int len;

	std::cout << "run" << std::endl;
	while (1)
    {
		memset(message, 0, 512);
        read_until_crlf(sock, message, &len);
		if (std::string(message).empty())
			continue ;
		split(std::string(message), ' ', split_ret);
		std::cout << "[RECV] " << message;
		std::string message_type = split_ret[1]; // PRIVMSG
		if (message_type == "PRIVMSG")
		{
			std::string command = get_command(message); // /help
			std::string target = get_target(message); // yochoi
			if (command == "/hello")
				hello_command(sock, target);
			if (command == "/help")
				help_command(sock, target, message);
			else
				unknown_command(sock, target, command);
		}
		if (message_type == "PING")
			write(sock, "notice bot :i am alive\n", 23);
		delete[] split_ret;
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
