#include "Socket.hpp"

Socket::Socket()
{
	memset(&_addr, 0, sizeof(_addr));
}

Socket::Socket(const char *port)
{
	_fd = socket(PF_INET, SOCK_STREAM, 0);
	if (_fd == -1)
		throw (Error("socket construct error"));

	memset(&_addr, 0, sizeof(_addr));
	_addr.sin_family = AF_INET;
	_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	_addr.sin_port = htons(ft::atoi(port));
}

Socket::Socket(unsigned short port)
{
	_fd = socket(AF_INET, SOCK_STREAM, 0);

	memset(&_addr, 0, sizeof(_addr));
	_addr.sin_family = AF_INET;
	_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	_addr.sin_port = port;
}

Socket::~Socket()
{
	close(_fd);
}

void	Socket::bind() const
{
	if (::bind(_fd, (struct sockaddr*)&_addr, sizeof(_addr)) == -1)
		throw (Error("socket bind error"));
}

void	Socket::listen() const
{
	if (::listen(_fd, 5) == -1)
		throw (Error("socket listen error"));
}

struct sockaddr_in	Socket::parsing_host_info(char *connect) const
{
	std::string *		split_ret;
	std::string			string_host;
	std::string			string_port_network;
	// password 관련 처리 방법 필요함 
	std::string			string_password_network;
	struct sockaddr_in	host;
	struct addrinfo		*result;

	ft::split(connect, ':', split_ret);
	string_host = split_ret[0];
	string_port_network = split_ret[1];
	string_password_network = split_ret[2];
	host.sin_family = AF_INET;
	host.sin_addr.s_addr = inet_addr(string_host.c_str());
	host.sin_port = htons(ft::atoi(string_port_network.c_str()));
	if (host.sin_addr.s_addr == -1)
		throw (Error("inet_addr() error"));
	if (getaddrinfo(string_host.c_str(), string_port_network.c_str(), NULL, &result) != 0)
		throw (Error("getaddrinfo() error"));
	freeaddrinfo(result);
	delete[] split_ret;
	return (host);
};

// 127.0.0.1:port:pass
Socket	*Socket::connect(char *connect_srv) const
{
	Socket				*new_sock;
	struct sockaddr_in	serv_addr;

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr = parsing_host_info(connect_srv);
	
	new_sock = new Socket(serv_addr.sin_port);
	if (new_sock->_fd == -1)
		throw (Error("connect socket create error"));
	if (::connect(new_sock->_fd, (struct sockaddr *)&new_sock->_addr, sizeof(new_sock->_addr)) == -1)
		throw (Error("socket connect error"));
	return (new_sock);
}

Socket*		Socket::accept() const
{
	Socket				*new_socket;
	struct sockaddr_in	client_addr;
	int					client_sock;
	socklen_t			clnt_addr_size;

	clnt_addr_size = sizeof(client_addr);
	client_sock = ::accept(_fd, (struct sockaddr*)&client_addr, &clnt_addr_size);
	if (client_sock < 0)
		throw (Error("socket accept error"));

	// copy
	new_socket = new Socket();
	new_socket->_fd = client_sock;
	memcpy(&new_socket->_addr, &client_addr, clnt_addr_size);
	return (new_socket);
}

void		Socket::write(char const *msg) const
{
	::write(_fd, msg, strlen(msg));
}

void		Socket::show_info() const
{
	std::cout << "==== Socket info ====" << std::endl;
	std::cout << "fd     : " << _fd << std::endl;
	std::cout << "type   : " << _type << std::endl;
	std::cout << "port to: " << get_port() << std::endl;
	std::cout << "=====================" << std::endl;
}

int			Socket::get_fd() const
{ return (_fd); }

unsigned short	Socket::get_port() const
{
	return (ntohs(_addr.sin_port));}

void			Socket::set_type(SockType::type type)
{
	_type = type;
}

SockType::type		Socket::get_type() const
{
	return (_type);
}

Socket::Socket(Socket const &copy) : _fd(copy._fd), _addr(copy._addr)
{
}

Socket&			Socket::operator=(Socket const &copy)
{
	if (_fd)
		close(_fd);
	_fd = copy._fd;
	_addr = copy._addr;
	return (*this);
}