#include "ft_irc.hpp"

IrcServer::IrcServer(int argc, char **argv)
{
	{
		ReadConf	rc;
		rc.open_file("ft_irc.conf");
		rc.read_config(this->_si);
	}
	if (argc == 3 || argc == 4)
	{
		_listen_socket = new Socket(htons(ft::atoi(argv[argc == 4 ? 2 : 1])));
		// std::cout << "=======\n";
		_listen_socket->set_type(LISTEN);
		_fd_max = _socket_set.add_socket(_listen_socket);
		_listen_socket->bind();
		_listen_socket->listen();
		if (_si.SERVER_NAME == "${AUTO}")
			_si.SERVER_NAME = std::string("test") + ft::itos(_listen_socket->get_port()) + ".com";
		add_server(_si.SERVER_NAME, "0", get_server_token(), ":" + _si.VERSION, _listen_socket);
		_my_pass = std::string(argv[argc == 4 ? 3 : 2]);
		time(&_start_time);
	}
	if (argc == 4)
		connect_to_server(argv);
};

IrcServer::~IrcServer()
{
};

// 새로운 서버에서 기존 서버로 연결하는 함수
void	 IrcServer::connect_to_server(char **argv)
{
	Socket			*new_socket;
	int				tmp;

	new_socket = _listen_socket->connect(argv[1]);
	new_socket->set_type(SERVER);
	tmp = _socket_set.add_socket(new_socket);
	if (_fd_max < tmp)
		_fd_max = tmp;
	_my_pass = std::string(argv[3]);
	// 이 시점에서 PASS 보내고
	std::string		msg = "PASS " + new_socket->get_pass() + "\n";
	new_socket->write(msg.c_str());
	msg = "SERVER " + _si.SERVER_NAME + " 1 :" + _si.VERSION + "\n";
	new_socket->write(msg.c_str());

	// 서버 내부 map에 있는 데이터를 send_msg로 전송해야 함
	// 자기 자신 포함해서 map 내부에 데이터를 SERVER 형태로 전송
	// Channel, USER도 마찬가지로 전송하는거 추가
	// send_map_data(_listen_socket->get_fd());
}

void	IrcServer::client_connect()
{
	Socket		*new_socket;

	new_socket = _listen_socket->accept();
	new_socket->set_type(UNKNOWN);
	_socket_set.add_socket(new_socket);
	// _user_map.insert(std::pair<unsigned short, int>(new_socket->get_port(), new_socket->get_fd()));
	if (_fd_max < new_socket->get_fd())
		_fd_max = new_socket->get_fd();
}

bool	IrcServer::check_pass(Socket *socket)
{
	if (socket->get_pass() == _my_pass)
		return (true);
	else
		return (false);
}

void	IrcServer::send_msg(int send_fd, const char *msg)
{
	if (DEBUG)
		std::cout << "send_msg(int, const char *) called." << std::endl;
	Socket	*socket = _socket_set.find_socket(send_fd);
	socket->write(msg);
}

void	IrcServer::send_msg_server(int fd, const char *msg)
{
	std::vector<Socket *> vec = _socket_set.get_connect_sockets();
	std::vector<Socket *>::iterator	begin = vec.begin();
	std::vector<Socket *>::iterator end = vec.end();

	while (begin != end)
	{
		Socket *s = (*begin);
		if (s->get_type() == SERVER && (s->get_fd() != fd))
			s->write(msg);
		begin++;
	}
}

void IrcServer::echo_msg(int my_fd, const char *buf, int len)
{
	for (int  i = 3; i < _fd_max + 1; i++)
	{
		// my_fd가 server인지 client인지 확인 후 정보 수정해서 전송
		// 현재 서버의 이름을 메시지의 경로에 추가
		Socket	*socket = _socket_set.find_socket(i);
		if (FD_ISSET(i, &(_socket_set.get_read_fds())) && i != my_fd && socket->get_type() != LISTEN && socket->get_type() != UNKNOWN)
		{
			if (DEBUG)
				std::cout << "echo_msg to fd: " << i << ", msg: " << buf << std::endl;
			socket->write(buf);
		}
	}
	write(1, buf, len);
}

void	IrcServer::send_map_data(int fd)
{
	std::map<std::string, Server *>::iterator begin;
	std::map<std::string, Server *>::iterator end;
	Server			*server;
	std::string		msg;

	// 서버 메세지 전송
	begin = _global_server.begin();
	end = _global_server.end();
	while (begin != end)// 전송하려는 포트 번호를 가진 fd에는 메시지를 보내지 않음
	{
		server = (*begin).second;
		msg = ":" + _si.SERVER_NAME + " SERVER " + server->get_name() + " " + ft::itos(server->get_hopcount() + 1) + " " + server->get_info() + "\n";
		send_msg(fd, msg.c_str());
		begin++;
	}
}

bool	IrcServer::is_reply_code(std::string const &command)
{
	int		num;

	if (command.length() == 3)
	{
		num = ft::atoi(command.c_str());
		if (num >= 200 && num < 600)
			return (true);
	}
	return (false);
}

void	IrcServer::update_last_time()
{
	std::vector<Socket *>	connects = _socket_set.get_connect_sockets();
	std::vector<Socket *>::iterator	begin = connects.begin();
	std::vector<Socket *>::iterator	end = connects.end();

	while (begin != end)
	{
		(*begin)->set_last_action();
		begin++;
	}
}

void	IrcServer::client_msg(int fd)
{
	char			buf[BUFFER_SIZE];
	int				str_len = 0;
	int				result;
	Command			*cmd;

	update_last_time();
	do
	{
		memset(buf, 0, BUFFER_SIZE);
		result = ft::read_until_crlf(fd, buf, &str_len);
		if (result == -1)
		{
			std::cout << "read_until_crlf return -1" << std::endl;
			return ;
		}
		std::cout << "[RECV] " << buf << " [" << fd << "] " << "[" << _current_sock->show_type() << "]\n";
		if (result == 2)
			return ;
		if (buf[0] == 0) // 클라이언트에서 Ctrl + C 입력한 경우
		{	// 해당 클라이언트와 연결 종료
			std::string msg;
			if (_current_sock->get_type() == SERVER)
			{
				cmd = _cmd_creator.get_command("SQUIT");
				cmd->set_message(NULL);
			}
			else
			{
				cmd = _cmd_creator.get_command("QUIT");
				Member *member = find_member(_current_sock->get_fd());
				if (member)
					msg = "QUIT :" + member->get_nick() + "\n";
				else
					msg = "QUIT\n";
				cmd->set_message(Message(msg.c_str()));
			}
			cmd->execute(*this);
			return ;
		}
		Message msg(buf);
		msg.set_source_fd(fd);
		cmd = _cmd_creator.get_command(msg.get_command());
		if (cmd)
		{
			cmd->set_message(msg);
			cmd->execute(*this);
			cmd->set_message(NULL);
			show_global_server();
			show_global_user();
			show_global_channel();
		}
		else
		{
			if (is_reply_code(msg.get_command()))
			{
				Member		*member;
				member = get_member(msg.get_param(0));
				member->get_socket()->write(msg.get_msg());
			}
			else if (buf[0] != '\n')
			{
				Reply::set_servername(_si.SERVER_NAME);
				Reply::set_username(find_member(_current_sock->get_fd())->get_nick());
				_current_sock->write(Reply(ERR::UNKNOWNCOMMAND(), msg.get_command()));
			}
		}
	} while (result);
}

void		IrcServer::fd_event_loop()
{
	struct timeval	timeout;
	fd_set	fds;
	int		fd_num;

	timeout.tv_sec = 1;
	timeout.tv_usec = 0;
	fds = _socket_set.get_read_fds_copy();
	if((fd_num = select(_fd_max + 1, &fds, 0 ,0, &timeout)) == -1)
		throw (Error(strerror(errno)));
	else if (fd_num != 0)
	{
		for (int i = 0; i < _fd_max + 1; i++)
		{
			if (FD_ISSET(i, &fds))
			{
				_current_sock = _socket_set.find_socket(i);
				if (_current_sock->get_type() == LISTEN)
				{
					client_connect();
					continue;
				}
				else
					client_msg(i);
			}
		}
	}
}

SocketSet	&IrcServer::get_socket_set()
{ return (_socket_set); }

void	IrcServer::check_connection()
{
	std::vector<Socket *>	connects = _socket_set.get_connect_sockets();
	std::vector<Socket *>::iterator	begin = connects.begin();
	std::vector<Socket *>::iterator	end = connects.end();


	time(&_current_time);
	while (begin != end)
	{
		if ((*begin)->get_type() == LISTEN)
			begin++;
		long	diff_time = _current_time - (*begin)->get_last_action();
		if (diff_time > 140)
		{
			// 해당 소켓 연결 종료
			// SQUIT이나 quit에 있는 내용 넣어두면 될 것 같음
		}
		else if (diff_time > 120)
		{
			//PING 전송
			// PING :<servername>
			std::string	msg = "PING :" + get_serverinfo().SERVER_NAME + "\n";
			if ((*begin)->get_type() == SERVER)
				msg = ":" + get_serverinfo().SERVER_NAME + " " + msg;
			send_msg((*begin)->get_fd(), msg.c_str());
		}
		begin++;
	}
}

void	IrcServer::run(int argc)
{
	while (1)
	{
		check_connection();
		fd_event_loop();
	}
}

// fd를 가지고 있는 멤버를 반환
Member		*IrcServer::find_member(int fd)
{
	std::map<std::string, Member *>::iterator begin = _global_user.begin();
	std::map<std::string, Member *>::iterator end = _global_user.end();

	while (begin != end)
	{
		if ((begin->second)->get_fd() == fd)
			return (begin->second);
		begin++;
	}
	return (NULL);
}

Member		*IrcServer::get_member(std::string nick)
{
	std::map<std::string, Member *>::iterator it = _global_user.find(nick);

	if (_global_user.end() == it)
		return (NULL);
	return (_global_user.find(nick)->second);
}

Channel		*IrcServer::get_channel(std::string channel_name)
{
	std::map<std::string, Channel *>::iterator it = _global_channel.find(channel_name);

	if (_global_channel.end() == it)
		return (NULL);
	else
		return (it->second);
}

// 해당 fd를 키로 가지는 값
Member		*IrcServer::get_member(int fd)
{
	std::string		key = ft::itos(fd);
	std::map<std::string, Member *>::iterator it = _global_user.find(key);

	if (_global_user.end() == it)
		return (NULL);
	return (_global_user.find(key)->second);
}

Socket		*IrcServer::get_current_socket()
{ return (_current_sock); }

Socket		*IrcServer::get_listen_socket()
{ return (_listen_socket); }

int			&IrcServer::get_fdmax()
{
	return (_fd_max);
}

std::map<std::string, Channel *>	&IrcServer::get_global_channel()
{
	return (_global_channel);
}

std::map<std::string, Member *>		&IrcServer::get_global_user()
{
	return(_global_user);
}

std::vector<Member>					&IrcServer::get_user_history()
{
	return (_user_history);
}

struct ServerInfo	&IrcServer::get_serverinfo()
{
	return (_si);
}

bool		IrcServer::add_member(std::string &nickname, Member *new_member)
{
	std::pair<std::map<std::string, Member *>::iterator, bool> result = 
		_global_user.insert(std::pair<std::string, Member *>(nickname, new_member));
	return (result.second);
}

void		IrcServer::delete_member(const std::string &nickname)
{
	_global_user.erase(nickname);
}

void		IrcServer::send_user_data(int fd)
{
	std::string									msg;

	std::map<std::string, Member *>::iterator	begin = _global_user.begin();
	std::map<std::string, Member *>::iterator	end = _global_user.end();

	while (begin != end)
	{
		// NICK, USER 순서로 제공된 fd에 메시지 전송
		msg = "NICK " + begin->second->get_nick() + " 1\n";;
		send_msg(fd, msg.c_str());
		msg = ":" + begin->second->get_nick() + " USER " +
			begin->second->get_username() + " " +
			begin->second->get_hostname() + " " +
			begin->second->get_servername() + " " +
			begin->second->get_realname() + "\n";
		send_msg(fd, msg.c_str());
		begin++;
	}
}

bool		IrcServer::add_server(const std::string &server_name, const std::string &hopcount, int token, const std::string &info, Socket *socket)
{
	Server	*new_server;

	new_server = new Server(server_name, hopcount, token, info);
	new_server->set_socket(socket);
	if (_global_server.insert(std::pair<std::string, Server *>(server_name, new_server)).second == false)
	{
		delete new_server;
		return (false);
	}
	else
		return (true);
}

void		IrcServer::delete_server(std::string const &server_name)
{
	_global_server.erase(server_name);
}

Server		*IrcServer::get_server(const std::string &server_name)
{
	std::map<std::string, Server *>::iterator	iter;

	iter = _global_server.find(server_name);
	if (iter != _global_server.end())
		return (iter->second);
	else
		return (0);
}

int			IrcServer::find_server_fd(const std::string &server_name)
{
	Server	*server;

	server = get_server(server_name);
	if (server)
		return (server->get_socket()->get_fd());
	else
		return (0);
}

std::map<std::string, Server *>		&IrcServer::get_global_server()
{
	return (_global_server);
}

void		IrcServer::add_channel(std::string &channel_name, Channel *channel)
{
	_global_channel.insert(std::pair<std::string, Channel *>(channel_name, channel));
}

void		IrcServer::delete_channel(const std::string &channel_name)
{
	_global_channel.erase(channel_name);
}

void		IrcServer::show_global_server()
{
	Server		*server;

	std::cout << "================== _global_server ======================\n";
	std::cout.width(20);
	std::cout << "server_name";
	std::cout.width(5);
	std::cout << "fd";
	std::cout.width(10);
	std::cout << "password";
	std::cout.width(5);
	std::cout << "hop";
	std::cout.width(20);
	std::cout << "info\n";

	std::map<std::string, Server *>::iterator	iter = _global_server.begin();
	while (iter != _global_server.end())
	{
		server = (*iter).second;

		std::cout.width(20);
		std::cout << server->get_name();

		std::cout.width(5);
		std::cout << server->get_socket()->get_fd();

		std::cout.width(10);
		std::cout << server->get_password();

		std::cout.width(5);
		std::cout << server->get_hopcount();
		
		std::cout.width(20);
		std::cout << server->get_info();
		std::cout << "\n";
		iter++;
	}
}

void		IrcServer::show_global_user()
{
	std::map<std::string, Member *>::iterator iter = _global_user.begin();

	std::cout << "================== _global_user ==================\n";
	std::cout.width(10);
	std::cout << "nickname";
	std::cout.width(10);
	std::cout << "username";
	std::cout.width(10);
	std::cout << "fd\n";
	while (iter != _global_user.end())
	{
		Member	*member = (*iter).second;
		std::cout.width(10);
		std::cout << member->get_nick();
		std::cout.width(10);
		std::cout << member->get_username();
		std::cout.width(10);
		std::cout << member->get_fd() << "\n";
		iter++;
	}
	return ;
}

#include <bitset>

void		IrcServer::show_global_channel()
{
	std::map<std::string, Channel *>::iterator iter = _global_channel.begin();
	std::vector<ChanMember>	member_vector;

	std::cout << "================== _global_channel ==================\n";
	std::cout.width(10);
	std::cout << "name";
	std::cout.width(10);
	std::cout << "topic";
	std::cout.width(20);
	std::cout << "mode";
	std::cout.width(10);
	std::cout << "users\n";
	while (iter != _global_channel.end())
	{
		std::cout.width(10);
		std::cout << (*iter).first;

		std::cout.width(10);
		std::cout << (*iter).second->get_topic();

		std::cout.width(20);
		std::cout << std::bitset<16>((*iter).second->get_mode()); // 사용 금지?

		member_vector = (*iter).second->get_members();
		std::vector<ChanMember>::iterator		member_iter;
		member_iter = member_vector.begin();
		while (member_iter != member_vector.end())
		{
			std::cout.width(10);
			std::string		temp;
			temp = (*member_iter)._member->get_nick();
			if ((*member_iter)._is_operator)
				temp += "(op)";
			std::cout << temp;
			member_iter++;
		}
		iter++;
		std::cout << "\n";
	}
	std::cout << "====================================================\n";
	return ;
}

int			IrcServer::get_server_token()
{
	std::map<std::string, Server *>::iterator	begin = _global_server.begin();
	std::map<std::string, Server *>::iterator	end = _global_server.end();
	int											token = 2;

	while (begin!= end)
	{
		if (token == begin->second->get_token())
		{
			begin = _global_server.begin();
			token++;
		}
		else
			begin++;
	}
	return (token);
}

// void				IrcServer::sigint_handler(int type)
// {
// 	std::string		msg;
// 	Command			*cmd;

// 	msg = "SQUIT" + _si.SERVER_NAME + " :SIGINT\n";
// 	cmd = _cmd_creator.get_command("SQUIT");
// 	cmd->set_message(Message(msg.c_str()));
// 	cmd->run(*this);
// 	// 사용한 메모리들 정리 작업 추가
// }

bool		IrcServer::check_oper(const std::string & id, const std::string & pwd)
{
	if (_si.OPERNAME == id && _si.OPERPWD == pwd)
		return (true);
	return (false);
}

time_t		IrcServer::get_start_time()
{ return (_start_time); }
