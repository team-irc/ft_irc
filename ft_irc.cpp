#include "ft_irc.hpp"

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

	// 서버 연결메시지 전송
	write(server_socket, "S", 1);
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

	// socket(프로토콜 체계, 소켓 타입, 내부 프로토콜);
	// 프로토콜 체계 : PF_INET(IPv4) PF_INET6(IPv6), ...
	// 소켓 타입 : SOCK_STREAM(연결지향), SOCK_DGRAM(비연결지향)
	// 최종적으로 사용할 프로토콜 : 프로토콜 내에 여러개의 프로토콜이 있는 경우가 있음, 보통 0 사용(그런 경우가 아니라면)
	_serv_sock = socket(AF_INET, SOCK_STREAM, 0);
	memset(&_serv_adr, 0, sizeof(_serv_adr));
	_serv_adr.sin_family = AF_INET;
	_serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	_serv_adr.sin_port = htons(atoi(argv[argc == 3 ? 2 : 1]));

	// bind: 소켓에 주소정보를 할당
	if (bind(_serv_sock, (struct sockaddr*) &_serv_adr, sizeof(_serv_adr)) == -1)
		error_handling("bind() error");

	// listen: 소켓을 연결 대기상태로 만듬.
	// backlog = 5, 클라이언트의 연결요청을 5개까지 대기시킬 수 있다.
	// 이 숫자는 실험적 결과에 의존해서 결정하게 된다. 웹서버의 경우엔 최소 15 이상이 되어야한다.
	if (listen(_serv_sock, 5) == -1)
		error_handling("listen() error");

	FD_ZERO(&_client_fds);
	FD_ZERO(&_server_fds);
	FD_SET(_serv_sock, &_server_fds);
	_fd_max = _serv_sock;
	
	if (argc == 3)
	{
		connect_to_server(argv);
	}
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
	std::istringstream	argument((std::string(argv[1])));
	std::string			string_host;
	std::string			string_port_network;
	// password 관련 처리 방법 필요함 
	std::string			string_password_network;
	struct sockaddr_in	host;
	struct addrinfo		*result;

	std::getline(argument, string_host, ':');
	std::getline(argument, string_port_network, ':');
	std::getline(argument, string_password_network, ':');
	host.sin_family = AF_INET;
	host.sin_addr.s_addr = inet_addr(string_host.c_str());
	host.sin_port = htons(atoi(string_port_network.c_str()));
	if (host.sin_addr.s_addr == -1)
		error_handling("inet_addr() error");
	if (getaddrinfo(string_host.c_str(), string_port_network.c_str(), NULL, &result) != 0)
		error_handling("getaddrinfo() error");
	freeaddrinfo(result);
	return (host);
};

void	IrcServer::client_connect()
{
	
	// ntohs(_client_adr.sin_port);
	// std::cout << "client_connect function called." << std::endl;
	socklen_t adr_sz = sizeof(_client_adr);
	// 
	_client_sock = accept(_serv_sock, (struct sockaddr*)&_client_adr, &adr_sz);
	FD_SET(_client_sock, &_client_fds);
	_user_map.insert(std::pair<unsigned short, int>(ntohs(_client_adr.sin_port), _client_sock));
	std::map<unsigned short, int>::iterator it = _user_map.find(ntohs(_client_adr.sin_port));
	std::cout << "connected client: " << _client_sock << std::endl;
	std::cout << "user_map.port: " << it->first << std::endl;
	std::cout << "user_map.value: " << it->second << std::endl;
	if (_fd_max < _client_sock)
		_fd_max = _client_sock;
	std::string msg = "user create:" + std::to_string(it->first);

	send_msg(_serv_sock, msg.c_str());
}

void IrcServer::send_msg(int my_fd, const char *msg)
{
	int size = strlen(msg);

	for (int i = 3; i < _fd_max + 1; i++)
	{
		if (i != my_fd && FD_ISSET(i, &_server_fds))
		{
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

void	IrcServer::client_msg(int fd)
{
	/*
	set_dest(message);
	{
		message.dest = Member.current_channel();
	}
	message.set_path(server.name);
	message.set_msg(buf);
	// 서버마다 연결된 유저들이 다름

	// 서버가 모든 채널에 대해 알고있고.
	// 채널은 자기에 속한 유저들을 다 알고있고.
	// 
	// 모든 서버가 이 메시지를 받고
	// 모든 서버가 각자 채널에 속한 유저들한테 메세지를 보낸다.
	//
	// 그러면, 유저가 처음에 서버에 접속할때
	// 닉네임 -> 유저커맨드 -> 서버접속 -> 채널JOIN -> 메세지
	//
	*/

	// std::cout << "client_msg function called." << std::endl;
	char	buf[BUF_SIZE];

	int str_len = read(fd, buf, BUF_SIZE);
	bool is_digit = false;
	std::istringstream	msg((std::string(buf)));
	std::string			user_port;
	// check server msg
	std::getline(msg, user_port, ':');
	std::getline(msg, user_port, ':');
	std::cout << "user_port:" << user_port << std::endl;
	for (char const &c : user_port) {
		if (std::isdigit(c) == 0) {
			break ;
		}
		is_digit = true;
	}
	if (is_digit)
	{
		_user_map.insert(std::pair<unsigned short, int>((unsigned short)atoi(user_port.c_str()), fd));
		std::map<unsigned short, int>::iterator it = _user_map.find((unsigned short)atoi(user_port.c_str()));
		std::cout << "new user_map.port: " << it->first << std::endl;
		std::cout << "new user_map.value: " << it->second << std::endl;
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
		// Member에서 제거도 해야 됨
	}
	else
		echo_msg(fd, buf, str_len);
}

// Member // User() 
// user.sock = accept()
// server.users = push(user);

// 모든 서버가 공유
// global_db.users = push(user);

// 닉네임검사 -> global_db.users 를 이용;

// 채널에 해당하는 유저들에게 메세지 전송 -> server.users를 이용

// 연결된 서버에 유저 생성 관련된 메시지 전송
// 


void	IrcServer::run(int argc)
{
	std::cout << "run function called." << std::endl;
	struct timeval	timeout;
	socklen_t		adr_sz;

	int fd_num;
	int i;
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
					// std::cout << i << " is set. (bbb)\n";
					if (i == _serv_sock)
					{
						std::cout << i << " is before connect" << std::endl;
						client_connect();
						std::cout << i << " is after connect" << std::endl;
					}
					else
					{
						client_msg(i);
					}
				}
			}
		}
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
					// std::cout << i << " is set. (aaa)\n";
					client_msg(i);
				}
			}
		}
	}
	close(_serv_sock);
};

void error_handling(const std::string buf)
{
	std::cerr << buf << std::endl;
	exit(1);
}