#ifndef SOCKET_HPP
# define SOCKET_HPP

# include <iostream>
# include <unistd.h>
# include <arpa/inet.h>
# include <sys/socket.h>
# include <string>
# include <sys/types.h>
# include <sys/socket.h>

class Socket
{
private:
	int					_fd;
	struct sockaddr_in	_addr;

public:
	Socket(const char *port);
	~Socket();
	Socket	*wait_connection();
};

Socket::Socket(const char *port)
{
	_fd = socket(PF_INET, SOCK_STREAM, 0);

	memset(&_addr, 0, sizeof(_addr));
	_addr.sin_family = AF_INET;
	_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	_addr.sin_port = htons(atoi(port));

	bind(_fd, (struct sockaddr*)&_addr, sizeof(_addr));
	listen(_fd, 5);
}

Socket::~Socket()
{
}

Socket*	Socket::wait_connection()
{
	Socket				*new_socket;
	struct sockaddr_in	client_addr;
	int					client_sock;
	socklen_t			clnt_addr_size;

	client_sock = accept(_fd, (struct sockaddr*)&client_addr, &clnt_addr_size);
	new_socket = new Socket(std::to_string(client_addr.sin_port).c_str());
	new_socket->_addr.sin_addr = client_addr.sin_addr;
	new_socket->_addr.sin_family = client_addr.sin_family;
	new_socket->_addr.sin_len = client_addr.sin_len;
	new_socket->_addr.sin_port = client_addr.sin_port;
	new_socket->_addr.sin_zero = client_addr.sin_zero;
	new_socket->_fd = client_sock;
}


#endif