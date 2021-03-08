#ifndef SOCKET_HPP
# define SOCKET_HPP

# include <iostream>
# include <unistd.h>
# include <arpa/inet.h>
# include <string>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include "Error.hpp"
# include "utils.hpp"

enum SOCKET_TYPE
{
	SERVER, CLIENT, LISTEN, UNKNOWN
};

class Socket
{
private:
	int					_fd;
	struct sockaddr_in	_addr;
	SOCKET_TYPE			_type;
	struct sockaddr_in	parsing_host_info(char *connect) const;
public:
	Socket();
	Socket(const char *port);
	Socket(unsigned short port);
	Socket(Socket const &copy);
	Socket& operator=(Socket const &copy);
	~Socket();
public:
	void			bind() const;
	void			listen() const;
	Socket			*accept() const;
	Socket			*connect(char *connect_srv) const;
	void			write(char const *msg) const;
	void			show_info() const;
	int				get_fd() const;
	unsigned short	get_port() const;
	void			set_type(SOCKET_TYPE type);
	SOCKET_TYPE		get_type() const;
	const char		*show_type() const;
};

#endif