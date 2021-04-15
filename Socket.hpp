#ifndef SOCKET_HPP
# define SOCKET_HPP

# include <iostream>
# include <unistd.h>
# include <arpa/inet.h>
# include <string>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <utility>
# include <time.h>
# include <fcntl.h>
# include "Error.hpp"
# include "utils.hpp"

class Reply;
class IrcServer;

enum SOCKET_TYPE
{
	SERVER, CLIENT, LISTEN, UNKNOWN
};

class Socket
{
private:
	int					_fd;
	std::string			_pass;
	struct sockaddr_in	_addr;
	SOCKET_TYPE			_type;
	time_t				_last_action;
private:
	std::pair<struct sockaddr_in, std::string>	parsing_host_info(const char *connect) const;
public:
	Socket();
	Socket(const char *port);
	Socket(unsigned short port);
	Socket(struct sockaddr_in serv_addr);
	Socket(Socket const &copy);
	Socket& operator=(Socket const &copy);
	~Socket();
public:
	void				bind() const;
	void				listen() const;
	Socket				*accept() const;
	Socket				*connect(const char *connect_srv);
	void				write(char const *msg) const;
	void				write(IrcServer &irc, Reply rpl) const;
	void				show_info() const;
	int					get_fd() const;
	unsigned short		get_port() const;
	const char			*get_hostname() const;
	void				set_type(SOCKET_TYPE type);
	SOCKET_TYPE			get_type() const;
	const char			*show_type() const;
	std::string const	&get_pass() const;
	void				set_pass(std::string const &val);

	time_t			get_last_action();
	void			set_last_action();
};

#endif