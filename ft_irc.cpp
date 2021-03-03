#include "ft_irc.hpp"

IrcServer::IrcServer(int argc, char **argv)
{
	if (DEBUG)
		std::cout << "Irc Server Constructor called." << std::endl;
	_listen_socket = new Socket(htons(ft::atoi(argv[argc == 3 ? 2 : 1])));
	_listen_socket->set_type(LISTEN);
	_fd_max = _socket_set.add_socket(_listen_socket);
	_user_map.insert(std::pair<unsigned short, int>(ntohs(_listen_socket->get_port()), _listen_socket->get_fd()));

	_listen_socket->bind();
	_listen_socket->listen();
	
	if (argc == 3)
		connect_to_server(argv);
};

IrcServer::~IrcServer()
{
};

void	 IrcServer::connect_to_server(char **argv)
{
	if (DEBUG)
		std::cout << "connect to server function called\n";
	Socket			*new_socket;
	int				tmp;

	new_socket = _listen_socket->connect(argv[1]);
	new_socket->set_type(SERVER);
	std::cout << "connect to server" << std::endl;
	tmp = _socket_set.add_socket(new_socket);
	// _socket_vector.push_back(new_socket);
	if (_fd_max < tmp)
		_fd_max = tmp;
	_user_map.insert(std::pair<unsigned short, int>(new_socket->get_port(), new_socket->get_fd()));

	// test
	std::cout << "connect_to_server\n";
	new_socket->show_info();

	// 서버 연결메시지 전송
	new_socket->write("S");
	sleep(5);

	// 서버 내부 map에 있는 데이터를 send_msg로 전송해야 함
	send_map_data(_listen_socket->get_fd());
}

/*
** 1번 서버에 2번 서버가 연결 요청
** socket함수 호출
** sockaddr_in 구조체에 값 채워넣음
** 1번 서버와 2번 서버 connect로 연결
** 연결 되면 password 확인
** password 틀리면 연결 끊고 종료
*/

struct sockaddr_in	IrcServer::parsing_host_info(char **argv)
{
	std::string *		split_ret;
	std::string			string_host;
	std::string			string_port_network;
	// password 관련 처리 방법 필요함 
	std::string			string_password_network;
	struct sockaddr_in	host;
	struct addrinfo		*result;

	ft::split(argv[1], ':', split_ret);
	string_host = split_ret[0];
	string_port_network = split_ret[1];
	string_password_network = split_ret[2];
	host.sin_family = AF_INET;
	host.sin_addr.s_addr = inet_addr(string_host.c_str());
	host.sin_port = htons(ft::atoi(string_port_network.c_str()));
	if (host.sin_addr.s_addr == -1)
		Error("inet_addr() error");
	if (getaddrinfo(string_host.c_str(), string_port_network.c_str(), NULL, &result) != 0)
		Error("getaddrinfo() error");
	freeaddrinfo(result);
	delete[] split_ret;
	return (host);
};

void	IrcServer::client_connect()
{
	if (DEBUG)
		std::cout << "client_connect function called." << std::endl;
	Socket		*new_socket;

	new_socket = _listen_socket->accept();
	new_socket->set_type(CLIENT);
	std::cout << "client connect" << std::endl;
	_socket_set.add_socket(new_socket);
	// _socket_vector.push_back(new_socket);
	_user_map.insert(std::pair<unsigned short, int>(new_socket->get_port(), new_socket->get_fd()));
	if (_fd_max < new_socket->get_fd())
		_fd_max = new_socket->get_fd();
	std::cout << "fd_max: " << _fd_max << std::endl;
	// test
	std::cout << "client_sock:" << new_socket->get_fd() << std::endl;
	new_socket->show_info();
	
	// user create 전송
	std::string msg = "user create:" + std::to_string(new_socket->get_port());
	std::cout << "=================================" << std::endl;

	// 자신에게 연결하는 클라이언트/서버에 대한 정보를 자신에 연결되어 있는 서버에 공유하기 위함
	for (int i = 3; i < _fd_max + 1; i++)
	{
		if (FD_ISSET(i, &_socket_set.get_server_fds()))
			send_msg(i, msg.c_str());
	}
	// send_msg(_listen_socket->get_fd(), new_socket->get_fd(), msg.c_str());
}

void	IrcServer::send_msg(int send_fd, const char *msg)
{
	if (DEBUG)
		std::cout << "send_msg(int, const char *) called." << std::endl;
	int		size = strlen(msg);

	std::cout << "send msg: " << msg << std::endl;
	write(send_fd, msg, size);
}


void IrcServer::send_msg(int my_fd, int except_fd, const char *msg)
{
	int size = strlen(msg);

	for (int i = 3; i < _fd_max + 1; i++)
	{
		if (i != my_fd && i != except_fd && FD_ISSET(i, &_socket_set.get_server_fds()))
		{
			std::cout << "send msg: " << msg << std::endl;
			write(i, msg, size);
		}
	}
}

void IrcServer::echo_msg(int my_fd, const char *buf, int len)
{
	if (DEBUG)
		std::cout << "echo_msg(int, char *, int) called." << std::endl;
	for (int  i = 3; i < _fd_max + 1; i++)
	{
		// my_fd가 server인지 client인지 확인 후 정보 수정해서 전송
		// 현재 서버의 이름을 메시지의 경로에 추가
		
		if (FD_ISSET(i, &(_socket_set.get_client_fds())) && i != my_fd)
		{
			if (DEBUG)
				std::cout << "echo_msg to client fd: " << i << ", msg: " << buf << std::endl;
			write(i, buf, len);
		}
		else if (FD_ISSET(i, &(_socket_set.get_server_fds())) && i != my_fd)
		{
			if (DEBUG)
				std::cout << "echo_msg to server fd: " << i << ", msg: " << buf << std::endl;
			write(i, buf, len);
		}
	}
	write(1, buf, len);
}

void	IrcServer::send_map_data(int my_fd)
{
	if (DEBUG)
		std::cout << "send_map_data called." << std::endl;
	std::map<unsigned short, int>::iterator begin;
	std::map<unsigned short, int>::iterator end;

	begin = _user_map.begin();
	end = _user_map.end();
	while (begin != end)
	{
		// 전송하려는 포트 번호를 가진 fd에는 메시지를 보내지 않음
		std::string msg = "user create:" + std::to_string(begin->first);
		for (int i = 3; i < _fd_max + 1; i++)
		{ 
			if ((begin)->second != i && FD_ISSET(i, &_socket_set.get_server_fds()))
				send_msg(i, msg.c_str());
		}
		begin++;
	}
}

void	IrcServer::show_map_data()
{
	std::map<unsigned short, int>::iterator begin = _user_map.begin();
	std::map<unsigned short, int>::iterator end = _user_map.end();

	std::cout << "============MAP DATA=============\n";
	while (begin != end)
	{
		std::cout << "key(port): " << begin->first << std::endl;
		std::cout << "value(fd): " << begin->second << std::endl;
		std::cout << "-----------------\n";
		begin++;
	}
	std::cout << "=================================\n";
}

namespace	ASCII_CONST
{
	const char		CR = 13;
	const char		LF = 10;
}

/*
** CR 또는 LF까지만 버퍼를 읽어온다
*/
static int	read_until_crlf(int fd, char *buffer)
{
	if (DEBUG)
		std::cout << "read_until_crlf start\n";
	int	i = 0;
	char	buf[BUFFER_SIZE];

	read(fd, buf, BUFFER_SIZE);
	for (i = 0; i < BUFFER_SIZE; i++)
	{
		if (buf[i] == ASCII_CONST::CR || buf[i] == ASCII_CONST::LF)
		{
			strncpy(buffer, buf, i + 1);
			buffer[i + 1] = 0;
			std::cout << "read crlf end\n";
			std::cout << buffer << std::endl;
			return (i);
		}
	}
	strncpy(buffer, buf, i + 1);
	buffer[i + 1] = 0;
	std::cout << "read crlf end\n";
	std::cout << buffer << std::endl;
	return (BUFFER_SIZE);
}

void	IrcServer::client_msg(int fd)
{
	if (DEBUG)
		std::cout << "client_msg function called." << std::endl;
	char			buf[BUFFER_SIZE];
	int				str_len = read_until_crlf(fd, buf);
	bool			is_digit = false;
	std::string *	split_ret;
	ft::split(std::string(buf), ':', split_ret);
	std::string		user_port = split_ret[1];
	const char *	tmp = user_port.c_str();

	// check server msg
	std::cout << "user_port:" << user_port << std::endl;
	for (int i = 0; i < user_port.length(); i++)
	{
		if (std::isdigit(tmp[i]) == false)
		{
			std::cout << "false:" << tmp[i] << std::endl;
			break ;
		}
		is_digit = true;
	}
	std::cout << "is_digit: " << is_digit << std::endl;
	if (is_digit)
	{
		_user_map.insert(std::pair<unsigned short, int>((unsigned short)ft::atoi(user_port.c_str()), fd));
		std::map<unsigned short, int>::iterator it = _user_map.find((unsigned short)ft::atoi(user_port.c_str()));
		//std::cout << "new user_map.port: " << it->first << std::endl;
		//std::cout << "new user_map.value: " << it->second << std::endl;
		show_map_data();
	}
	if (str_len == 0)
	{
		_socket_set.remove_socket(_socket_set.find_socket(fd));
		close(fd);
		std::cout << "closed client: " << fd << std::endl;
	}
	else if (str_len == 1 && buf[0] == 'S')
	{
		std::cout << "receive S msg" << std::endl;
		Socket *tmp = _socket_set.find_socket(fd);
		std::cout << "find Socket: "  << tmp->get_port() << std::endl;
		_socket_set.remove_socket(tmp);
		tmp->set_type(SERVER);
		_socket_set.add_socket(tmp);

		/*
		_user_map.insert(std::pair<unsigned short, int>(new_socket->get_port(), new_socket->get_fd()));
		*/

		// FD_SET(fd, &_socket_set.get_server_fds());

		// std::vector<Socket *>::iterator		iter;
		// for (iter = _socket_vector.begin(); iter != _socket_vector.end(); iter++)
		// {
		// 	if ((*iter)->get_fd() == fd)
		// 	{
		// 		(*iter)->set_type(SERVER);
		// 	}
		// }
		// 새로운 서버가 연결을 시도하는 경우(이미 연결은 됐고 서버로 인증받는 단계)
		// fd는 새롭게 연결되는 서버고, 이 서버에는 기존 서버들에 대한 정보가 필요함
		// map에 있는 데이터들을 전송해줘서 해당 서버가 연결하기 위한 통로를 알 수 있도록 메시지 전송
		send_map_data(_listen_socket->get_fd());
		// Member에서 제거도 해야 됨
	}
	else
	{
		Message msg(buf);
		msg.get_info();
		echo_msg(fd, msg.get_msg(), msg.get_size());
		// echo_msg(fd, buf, str_len);
	}
	delete[] split_ret;
}

void	IrcServer::manage_listen(struct timeval &timeout)
{
	fd_set	listen_fd;
	int		fd_num;
	
	listen_fd = _socket_set.get_listen_fds();
	for (int i = 3; i < _fd_max + 1; i++)
	{
		if (FD_ISSET(i, &listen_fd))
			std::cout << "listen_fd:" << i << std::endl;
	}
	if((fd_num = select(_fd_max + 1, &listen_fd, 0 ,0, &timeout)) == -1)
		throw (Error("select return -1. listen fd"));
	if (fd_num != 0)
	{
		if (DEBUG)
			std::cout << "manage_listen called.\n";
		for (int i = 0; i < _fd_max + 1; i++)
		{
			if (FD_ISSET(i, &listen_fd))
			{
				client_connect();
			}
		}
	}
}

void	IrcServer::manage_server(struct timeval &timeout)
{

	fd_set	server_fd;
	int		fd_num;

	server_fd = _socket_set.get_server_fds();
	std::cout << "_fd_max: " << _fd_max << std::endl;
	for (int i = 3; i < _fd_max + 1; i++)
	{
		if (FD_ISSET(i, &server_fd))
			std::cout << "server_fd:" << i << std::endl;
	}
	if ((fd_num = select(_fd_max + 1, &server_fd, 0, 0, &timeout)) == -1)
		throw (Error("select return -1. server_fds"));
	else if (fd_num != 0)
	{
		if (DEBUG)
			std::cout << "manage_server called.\n";
		for (int i = 0; i < _fd_max + 1; i++)
		{
			if (FD_ISSET(i, &server_fd))
			{
				client_msg(i);
			}
		}
	}
}

void	IrcServer::manage_client(struct timeval &timeout)
{
	fd_set	client_fd;
	int		fd_num;
	
	client_fd = _socket_set.get_client_fds();
	for (int i = 3; i < _fd_max + 1; i++)
	{
		if (FD_ISSET(i, &client_fd))
			std::cout << "client_fd:" << i << std::endl;
	}
	if((fd_num = select(_fd_max + 1, &client_fd, 0 ,0, &timeout)) == -1)
		throw (Error("select return -1. client fd"));
	else if (fd_num != 0)
	{
		if (DEBUG)
			std::cout << "manage_client called.\n";
		for (int i = 0; i < _fd_max + 1; i++)
		{
			if (FD_ISSET(i, &client_fd))
			{
				// 메세지 처리 (클라이언트 인 경우) -> 서버 검증 (Y) -> 유저맵 전송 
				//                                      (N) -> echo_msg
				client_msg(i);
			}
		}
	}
}

void	IrcServer::run(int argc)
{
	if (DEBUG)
		std::cout << "run function called." << std::endl;
	struct timeval	timeout;

	timeout.tv_sec = 5;
	timeout.tv_usec = 5000;
	while (1)
	{
		if (DEBUG)
		{
			std::cout << "===============Current Info=================\n";
			show_map_data();
			_socket_set.show_info();
			std::cout << "============================================\n";
		}
		manage_listen(timeout);
		manage_server(timeout);
		manage_client(timeout);
	}
};