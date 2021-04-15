#include "Socket.hpp"
#include "Reply.hpp"

Socket::Socket()
{
	memset(&_addr, 0, sizeof(_addr));
}

Socket::Socket(const char *port)
{
	_fd = socket(PF_INET, SOCK_STREAM, 0);
	if (_fd == -1)
		throw(Error("socket construct error"));

	memset(&_addr, 0, sizeof(_addr));
	_addr.sin_family = AF_INET;
	_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	_addr.sin_port = htons(ft::atoi(port));
}

Socket::Socket(unsigned short port)
{
	_fd = socket(AF_INET, SOCK_STREAM, 0);

	memset(&_addr, 0, sizeof(_addr));
	_addr.sin_family = AF_INET;
	_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	_addr.sin_port = port;
}

Socket::Socket(struct sockaddr_in serv_addr)
{
	_fd = socket(AF_INET, SOCK_STREAM, 0);

	memset(&_addr, 0, sizeof(_addr));
	_addr = serv_addr;
}

Socket::Socket(Socket const &copy) : _fd(copy._fd), _addr(copy._addr)
{
}

Socket &Socket::operator=(Socket const &copy)
{
	if (_fd)
		close(_fd);
	_fd = copy._fd;
	_addr = copy._addr;
	return (*this);
}

Socket::~Socket()
{
	close(_fd);
}

void Socket::bind() const
{
	if (::bind(_fd, (struct sockaddr *)&_addr, sizeof(_addr)) == -1)
		throw(Error("socket bind error"));
}

void Socket::listen() const
{
	if (::listen(_fd, 5) == -1)
		throw(Error("socket listen error"));
}

/*
** 1번 서버에 2번 서버가 연결 요청
** socket함수 호출
** sockaddr_in 구조체에 값 채워넣음
** 1번 서버와 2번 서버 connect로 연결
** 연결 되면 password 확인
** password 틀리면 연결 끊고 종료
*/

// first: host
// second: password
std::pair<struct sockaddr_in, std::string> Socket::parsing_host_info(const char *connect) const
{
	std::string *split_ret;
	std::string string_host;
	std::string string_port_network;
	std::string string_password_network;
	struct sockaddr_in host;

	if (ft::split(connect, ':', split_ret) != 3)
		throw(Error("invalid host info"));
	string_host = split_ret[0];
	string_port_network = split_ret[1];
	string_password_network = split_ret[2];
	host.sin_family = AF_INET;
	host.sin_addr.s_addr = inet_addr(string_host.c_str());
	host.sin_port = htons(ft::atoi(string_port_network.c_str()));
	if (host.sin_addr.s_addr == -1)
		throw(Error("inet_addr() error"));
	delete[] split_ret;
	return (std::make_pair(host, string_password_network));
};

//110110011111010100000000100010
//54 125 64 34

// 127.0.0.1:port:pass
Socket *Socket::connect(const char *connect_srv)
{
	Socket *new_sock;
	struct sockaddr_in serv_addr;
	std::pair<struct sockaddr_in, std::string> pair;

	memset(&serv_addr, 0, sizeof(serv_addr));
	pair = parsing_host_info(connect_srv);
	serv_addr = pair.first;

	// new_sock = new Socket(serv_addr.sin_port);
	new_sock = new Socket(serv_addr);
	new_sock->set_pass(pair.second);
	if (new_sock->_fd == -1)
		throw(Error("connect socket create error"));
	if (::connect(new_sock->_fd, (struct sockaddr *)&new_sock->_addr, sizeof(new_sock->_addr)) == -1)
		throw(Error("socket connect error"));
	return (new_sock);
}

Socket *Socket::accept() const
{
	Socket *new_socket;
	struct sockaddr_in client_addr;
	int client_sock;
	socklen_t clnt_addr_size;

	clnt_addr_size = sizeof(client_addr);
	client_sock = ::accept(_fd, (struct sockaddr *)&client_addr, &clnt_addr_size);
	if (client_sock < 0)
		throw(Error("socket accept error"));

	// copy
	new_socket = new Socket();
	new_socket->_fd = client_sock;
	memcpy(&new_socket->_addr, &client_addr, clnt_addr_size);
	return (new_socket);
}

void Socket::write(char const *msg) const
{
	std::cout << "[SEND] " << msg << " [" << _fd << "] "
			  << "[" << show_type() << "]\n";
	::write(_fd, msg, strlen(msg));
}

void Socket::write(Reply rpl) const
{
	std::cout << "[SEND] " << rpl.get_msg().c_str() << " [" << _fd << "] "
			  << "[" << show_type() << "]\n";
	::write(_fd, rpl.get_msg().c_str(), strlen(rpl.get_msg().c_str()));
}

void Socket::show_info() const
{
	std::cout << "==== Socket info ====" << std::endl;
	std::cout << "fd     : " << _fd << std::endl;
	std::cout << "type   : " << show_type() << std::endl;
	std::cout << "port to: " << get_port() << std::endl;
	std::cout << "=====================" << std::endl;
}

int Socket::get_fd() const
{
	return (_fd);
}

unsigned short Socket::get_port() const
{
	return (ntohs(_addr.sin_port));
}

const char *Socket::get_hostname() const
{
	return (inet_ntoa(_addr.sin_addr));
}

std::string const &Socket::get_pass() const
{
	return (_pass);
}

void Socket::set_pass(std::string const &val) { _pass = val; }

void Socket::set_type(SOCKET_TYPE type)
{
	_type = type;
}

SOCKET_TYPE Socket::get_type() const
{
	return (_type);
}

const char *Socket::show_type() const
{
	if (_type == SERVER)
		return ("Server");
	else if (_type == CLIENT)
		return ("Client");
	else if (_type == LISTEN)
		return ("Listen");
	else if (_type == UNKNOWN)
		return ("Unknown");
	else
		return ("not defined type");
}