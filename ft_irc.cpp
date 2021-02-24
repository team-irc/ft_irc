#include "ft_irc.hpp"
#include "utils.hpp"

void	 IrcServer::connect_to_server(char **argv)
{
	int		server_socket;

	// std::cout << "connect to server function called\n";
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket == -1)
		error_handling("socket() error");

	memset(&_client_adr, 0, sizeof(_client_adr));
	_client_adr = parsing_host_info(argv);
	// if (_client_adr == NULL)
	// 	error_handling("server info error");
	if (connect(server_socket, (struct sockaddr *)&_client_adr, sizeof(_client_adr)) == -1)
		error_handling("connect error");
	FD_SET(server_socket, &_server_fds);
	std::cout << "key:" << ntohs(_client_adr.sin_port) << " val:" << server_socket << std::endl;
	_user_map.insert(std::pair<unsigned short, int>(ntohs(_client_adr.sin_port), server_socket));
	// 서버 연결메시지 전송
	write(server_socket, "S", 1);
	// _serv_sock = server_socket;

	// 서버 내부 map에 있는 데이터를 send_msg로 전송해야 함
	send_map_data(_serv_sock);
	_fd_max++;
}

IrcServer::IrcServer(int argc, char **argv)
{
	// std::cout << "Irc Server Constructor called." << std::endl;
	char buf[BUF_SIZE];

	// {
	// 	std::cout << "Usage: " << argv[0] << " <port>" << std::endl;
	// 	exit(1);
	// }

	//fd 3
	_serv_sock = socket(AF_INET, SOCK_STREAM, 0);
	memset(&_serv_adr, 0, sizeof(_serv_adr));
	_serv_adr.sin_family = AF_INET;
	_serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	_serv_adr.sin_port = htons(ft::atoi(argv[argc == 3 ? 2 : 1]));

	if (bind(_serv_sock, (struct sockaddr*) &_serv_adr, sizeof(_serv_adr)) == -1)
		error_handling("bind() error");

	if (listen(_serv_sock, 5) == -1)
		error_handling("listen() error");

	FD_ZERO(&_client_fds);
	FD_ZERO(&_server_fds);
	FD_SET(_serv_sock, &_server_fds);
	_fd_max = _serv_sock;
	_user_map.insert(std::pair<unsigned short, int>(ntohs(_serv_adr.sin_port), _serv_sock));
	if (argc == 3)
		connect_to_server(argv);
};

IrcServer::~IrcServer()
{
};

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

	split_ret = ft::split(argv[1], ':');
	string_host = split_ret[0];
	string_port_network = split_ret[1];
	string_password_network = split_ret[2];
	host.sin_family = AF_INET;
	host.sin_addr.s_addr = inet_addr(string_host.c_str());
	host.sin_port = htons(ft::atoi(string_port_network.c_str()));
	if (host.sin_addr.s_addr == -1)
		error_handling("inet_addr() error");
	if (getaddrinfo(string_host.c_str(), string_port_network.c_str(), NULL, &result) != 0)
		error_handling("getaddrinfo() error");
	freeaddrinfo(result);
	delete[] split_ret;
	return (host);
};

void	IrcServer::client_connect()
{
	// std::cout << "client_connect function called." << std::endl;
	socklen_t adr_sz;

	adr_sz = sizeof(_client_adr);
	if ((_client_sock = accept(_serv_sock, (struct sockaddr*)&_client_adr, &adr_sz)) == -1)
		std::cout << "EAGAIN" << std::endl;
	else
		std::cout << "SUCCESS" << std::endl;
	FD_SET(_client_sock, &_client_fds);
	std::cout << "client_sock:" << _client_sock << std::endl;
	_user_map.insert(std::pair<unsigned short, int>(ntohs(_client_adr.sin_port), _client_sock));
	std::map<unsigned short, int>::iterator it = _user_map.find(ntohs(_client_adr.sin_port));

	std::cout << "connected client: " << _client_sock << std::endl;
	std::cout << "user_map.port: " << ntohs(_client_adr.sin_port) << " server:" << ntohs(_serv_adr.sin_port) << std::endl;
	std::cout << "user_map.value: " << it->second << std::endl;
	if (_fd_max < _client_sock)
		_fd_max = _client_sock;
	std::string msg = "user create:" + std::to_string(it->first);
	std::cout << "=================================" << std::endl;
	

	// 자신에게 연결하는 클라이언트/서버에 대한 정보를 자신에 연결되어 있는 서버에 공유하기 위함
	send_msg(_serv_sock, it->second, msg.c_str());
}

void IrcServer::send_msg(int my_fd, int except_fd, const char *msg)
{
	int size = strlen(msg);

	for (int i = 3; i < _fd_max + 1; i++)
	{
		if (i != my_fd && i != except_fd && FD_ISSET(i, &_server_fds))
		{
			std::cout << "send msg: " << msg << std::endl;
			write(i, msg, size);
		}
	}
}

void IrcServer::echo_msg(int my_fd, char *buf, int len)
{
	// std::cout << "echo_msg function called." << std::endl;
	for (int  i = 3; i < _fd_max + 1; i++)
	{
		// my_fd가 server인지 client인지 확인 후 정보 수정해서 전송
		// 현재 서버의 이름을 메시지의 경로에 추가
		if (FD_ISSET(i, &_client_fds) && i != my_fd)
		{
			write(i, buf, len);
		}
		else if (FD_ISSET(i, &_server_fds) && i != my_fd)
		{
			write(i, buf, len);
		}
	}
	write(1, buf, len);
}

void	IrcServer::send_map_data(int my_fd)
{
	std::map<unsigned short, int>::iterator begin;
	std::map<unsigned short, int>::iterator end;

	begin = _user_map.begin();
	end = _user_map.end();
	while (begin != end)
	{
		// 전송하려는 포트 번호를 가진 fd에는 메시지를 보내지 않음
		std::string msg = "user create:" + std::to_string(begin->first);
		send_msg(my_fd, begin->second, msg.c_str());
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

void	IrcServer::client_msg(int fd)
{
	// std::cout << "client_msg function called." << std::endl;
	char				buf[BUF_SIZE];
	int					str_len = read(fd, buf, BUF_SIZE);
	bool				is_digit = false;
	std::string *		split_ret = ft::split(std::string(buf), ':');
	std::string			user_port = split_ret[1];
	const char *		tmp = user_port.c_str();

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
		FD_CLR(fd, &_client_fds);
		close(fd);
		std::cout << "closed client: " << fd << std::endl;
	}
	else if (str_len == 1 && buf[0] == 'S')
	{
		FD_CLR(fd, &_client_fds);
		FD_SET(fd, &_server_fds);
		// 새로운 서버가 연결을 시도하는 경우(이미 연결은 됐고 서버로 인증받는 단계)
		// fd는 새롭게 연결되는 서버고, 이 서버에는 기존 서버들에 대한 정보가 필요함
		// map에 있는 데이터들을 전송해줘서 해당 서버가 연결하기 위한 통로를 알 수 있도록 메시지 전송
		send_map_data(_serv_sock);
		// Member에서 제거도 해야 됨
	}
	else
		echo_msg(fd, buf, str_len);
	delete[] split_ret;
}

void	IrcServer::run(int argc)
{
	std::cout << "run function called." << std::endl;
	struct timeval	timeout;
	socklen_t		adr_sz;
	int				fd_num;
	int				i;
	fd_set			tmp;

	while (1)
	{
		timeout.tv_sec = 5;
		timeout.tv_usec = 5000;

		tmp = _server_fds;
		if ((fd_num = select(_fd_max+1, &tmp, 0, 0, &timeout)) == -1)
		{
			std::cout << "select return -1. server_fds" << std::endl;
			break;
		}
		if (fd_num != 0)
		{
			for (i = 0; i < _fd_max + 1; i++)
			{
				if (FD_ISSET(i, &tmp))
				{
					if (i == _serv_sock)
					{
						// 새로운 연결 요청이 온 경우 -> 새로운 유저 생성 (맵에 추가) -> 다른 서버에 유저 생성 메세지 전송
						// (다른 서버가 처음 요청 한 경우 (argc == 3) -> 일단 클라이언트라고 가정, client_msg에서 검증하고 변경함)
						client_connect();
					}
					else
					{
						// 메세지 처리 (서버) -> (유저 생성 메세지?) (Y) -> 유저 맵에 추가 -> 다른 서버로 전파
						//									 (N) -> echo_msg

						client_msg(i);
					}
				}
			}
		}
		//run(tmp, 함수포인터);

		/*
		if ((fd_num = select(_fd_max+1, &tmp, 0, 0, &timeout)) == -1)
		{
			std::cout << "select return -1. server_fds" << std::endl;
			break;
		}
		if (fd_num != 0)
		{
			for (i = 0; i < _fd_max + 1; i++)
			{
				함수 포인터 실행
			}
		}

		server_run
		{

		}
		client_run
		{

		}
		*/
		tmp = _client_fds;
		if((fd_num = select(_fd_max+1, &tmp, 0 ,0, &timeout)) == -1)
		{
			std::cout << "select return -1. client fd" << std::endl;
			break;
		}
		if (fd_num != 0)
		{
			for (i = 0; i < _fd_max + 1; i++)
			{
				if (FD_ISSET(i, &tmp))
				{
					// 메세지 처리 (클라이언트 인 경우) -> 서버 검증 (Y) -> 유저맵 전송 
					//                                      (N) -> echo_msg
					client_msg(i);
				}
			}
		}
	}
	close(_serv_sock);
};

