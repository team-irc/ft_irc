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
# ifdef __linux__
# include <errno.h>
# endif
# include "Error.hpp"
# include "utils.hpp"

class Reply;
class IrcServer;

enum SOCKET_TYPE
{
	SERVER, CLIENT, LISTEN, SSL_LISTEN, UNKNOWN
};

class Socket
{
protected:
	int					_fd;
	std::string			_pass;
	std::string			_linkname;
	struct sockaddr_in	_addr;
	SOCKET_TYPE			_type;

	size_t				_recv_bytes;
	size_t				_recv_cnt;
	size_t				_sent_bytes;
	size_t				_sent_cnt;

	time_t				_start_time;
	time_t				_last_action;
	bool				_is_ping_check;
	
public:
	Socket();
	Socket(const char *port);
	Socket(unsigned short port);
	Socket(struct sockaddr_in serv_addr);
	Socket(Socket const &copy);
	Socket& operator=(Socket const &copy);
	virtual ~Socket();
public:
	void				bind() const;
	void				listen() const;
	virtual Socket		*accept() const;
	static Socket		*connect(const char *connect_srv);
	virtual int			read(int fd, char *buffer, int *len);
	virtual void		write(char const *msg);
	virtual void		write(Reply rpl);
	void				show_info() const;
	int					get_fd() const;
	unsigned short		get_port() const;
	const char			*get_hostname() const;
	void				set_type(SOCKET_TYPE type);
	SOCKET_TYPE			get_type() const;
	const char			*show_type() const;
	std::string const	&get_pass() const;
	void				set_pass(std::string const &val);
	size_t				get_sent_bytes();
	size_t				get_recv_bytes();
	size_t				get_sent_cnt();
	size_t				get_recv_cnt();
	void				set_linkname(std::string const &linkname);
	std::string			get_linkname();

	time_t			get_start_time();
	time_t			get_last_action();
	void			set_last_action();

	bool			is_ping_check();
	void			set_ping_check();
	static std::pair<struct sockaddr_in, std::string>	parsing_host_info(const char *connect);
};

#endif