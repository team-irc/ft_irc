#include "ft_irc.hpp"

void IrcServer::echo_msg(int my_fd, char *buf, int len)
{
	for (int  i = 3; i < _fd_max + 1; i++)
	{
		if (i != my_fd)
			write(i, buf, len);
	}
	write(1, buf, len);
}

IrcServer::IrcServer(int argc, char **argv)
{
	char buf[BUF_SIZE];

	// {
	// 	std::cout << "Usage: " << argv[0] << " <port>" << std::endl;
	// 	exit(1);
	// }

	// socket(프로토콜 체계, 소켓 타입, 내부 프로토콜);
	// 프로토콜 체계 : PF_INET(IPv4) PF_INET6(IPv6), ...
	// 소켓 타입 : SOCK_STREAM(연결지향), SOCK_DGRAM(비연결지향)
	// 최종적으로 사용할 프로토콜 : 프로토콜 내에 여러개의 프로토콜이 있는 경우가 있음, 보통 0 사용(그런 경우가 아니라면)
	_serv_sock=socket(AF_INET, SOCK_STREAM, 0);
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

	FD_ZERO(&_reads);
	FD_SET(_serv_sock, &_reads);
	_fd_max = _serv_sock;
	if (argc == 3)
	{
		_client_sock = socket(AF_INET, SOCK_STREAM, 0);
		if (_client_sock == -1)
			error_handling("socket() error");
		memset(&_client_adr, 0, sizeof(_client_adr));
		_client_adr = parsing_host_info(argv);
		// if (_client_adr == NULL)
		// 	error_handling("server info error");
		if (connect(_client_sock, (struct sockaddr *)&_client_adr, sizeof(_client_adr)) == -1)
			error_handling("connect error");
		FD_SET(_client_sock, &_reads);
		_fd_max++;
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
	std::istringstream argument((std::string(argv[1])));
	std::string string_host;
	std::string string_port_network;
	// password 관련 처리 방법 필요함 
	std::string string_password_network;
	struct sockaddr_in host;
	struct addrinfo *result;

	std::getline(argument, string_host, ':');
	std::getline(argument, string_port_network, ':');
	std::getline(argument, string_password_network, ':');
	host.sin_family = AF_INET;
	host.sin_addr.s_addr = inet_addr(string_host.c_str());
	host.sin_port = htons(atoi(string_port_network.c_str()));
	if (host.sin_addr.s_addr == -1)
		error_handling("oh my god");
	if (getaddrinfo(string_host.c_str(), string_port_network.c_str(), NULL, &result) != 0)
		error_handling("shit");
	freeaddrinfo(result);
	return (host);
};

void	IrcServer::client_connect()
{
	socklen_t adr_sz = sizeof(_client_adr);
	_client_sock = accept(_serv_sock, (struct sockaddr*)&_client_adr, &adr_sz);
	FD_SET(_client_sock, &_reads);
	std::cout << "connected client: " << _client_sock << std::endl;
	if (_fd_max < _client_sock)
		_fd_max = _client_sock;
}

//sever

void	IrcServer::client_msg(int fd)
{
	char	buf[BUF_SIZE];

	int str_len = read(fd, buf, BUF_SIZE);
	if (str_len == 0)
	{
		FD_CLR(fd, &_reads);
		close(fd);
		std::cout << "closed client: " << fd << std::endl;
	}
	else
		echo_msg(fd, buf, str_len);
}

void	IrcServer::run()
{
	struct timeval	timeout;
	socklen_t		adr_sz;

	int fd_num;
	int i;

	_fd_max = _serv_sock;

	while (1)
	{
		_cpy_reads = _reads;
		timeout.tv_sec = 5;
		timeout.tv_usec = 5000;

		if((fd_num = select(_fd_max+1, &_cpy_reads, 0 ,0, &timeout)) == -1)
			break;
		if (fd_num == 0)
			continue;
		for (i = 0; i < _fd_max + 1; i++)
		{
			if (FD_ISSET(i, &_cpy_reads))
			{
				if (i == _serv_sock)
				{
					client_connect();
				}
				else
				{
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