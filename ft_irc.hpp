#ifndef FT_IRC_HPP
#define FT_IRC_HPP

#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define BUF_SIZE 520

class IrcServer
{
private:

	int					_serv_sock;
	int					_client_sock;
	struct	sockaddr_in _serv_adr;
	struct	sockaddr_in _client_adr;
	socklen_t			_client_adr_size;
	fd_set				_reads;
	fd_set				_cpy_reads;
	int					_fd_max;
public:

	IrcServer(int argc, char **argv);
	virtual ~IrcServer();

public:

	void	run();

private:

	void				echo_msg(int my_fd, char *buf, int len);
	void				client_msg(int fd);
	void				client_connect();
	struct sockaddr_in	parsing_host_info(char **argv);

};

void error_handling(const std::string buf);

#endif