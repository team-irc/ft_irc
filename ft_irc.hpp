#ifndef FT_IRC_HPP
# define FT_IRC_HPP

#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <map>

#define BUF_SIZE 512
#include "Socket.hpp"
#include "SocketSet.hpp"
#include "utils.hpp"
#include "Error.hpp"
#include "Message.hpp"
// #include "Member.hpp"
// #include "Channel.hpp"

#define DEBUG 1

class IrcServer
{
private:
	
	Socket					*_listen_socket;
	// std::vector<Socket *>	_socket_vector;
	SocketSet				_socket_set;
	
	int						_fd_max;
	
	std::map<unsigned short, int>	_user_map;

public:

	IrcServer(int argc, char **argv);
	virtual ~IrcServer();

public:

	void	run(int argc);

private:

	void				echo_msg(int my_fd, const char *buf, int len);
	void				client_msg(int fd);
	void				client_connect();
	struct sockaddr_in	parsing_host_info(char **argv);
	void				connect_to_server(char **argv);
	void				send_msg(int send_fd, const char *msg);
	void				send_msg(int my_fd, int except_fd, const char *msg);
	void				send_map_data(int my_fd);
	void				show_map_data();
	void				manage_server(struct timeval &timeout);
	void				manage_client(struct timeval &timeout);
	void				manage_listen(struct timeval &timeout);

};

void error_handling(const std::string buf);

#endif