#include "SquitCommand.hpp"

// 해당 fd값을 가진 멤버를 제거 + 모든 서버에 QUIT 메세지 전달

static void		quit_from_joined_channel(IrcServer &irc, Member	*member)
{
	std::set<Channel *>::iterator	channel_iter;
	Channel		*channel;

	channel_iter = member->get_joined_channels().begin();
	while (channel_iter != member->get_joined_channels().end())
	{
		channel = (*channel_iter);
		channel->delete_member(member); // 채널의 멤버 목록에서 제거
		if (channel->get_members().empty()) 
		{
			irc.delete_channel(channel->get_name()); // _global_channel에서 제거
			delete channel; // 채널 인스턴스 제거
		}
		++channel_iter;
	}
}

void	SquitCommand::send_quit_user(int fd, IrcServer &irc)
{
	Member			*member;
	std::string		msg;

	while ((member = irc.find_member(fd)))
	{
		msg = ":" + member->get_nick() + " QUIT :server connect out\n";
		irc.send_msg_server(fd, msg.c_str());

		irc.delete_member(member->get_nick());
		quit_from_joined_channel(irc, member);
		irc.get_user_history().push_back(*member);
		irc.delete_member(member->get_nick());
		delete (member);
	}
}

// 해당 fd값을 가진 서버 제거 후 SQUIT 전송
void	SquitCommand::delete_server(int fd, IrcServer &irc)
{
	std::map<std::string, Server *>::iterator	begin = irc.get_global_server().begin();
	std::map<std::string, Server *>::iterator	end = irc.get_global_server().end();
	std::map<std::string, Server *>::iterator	tmp;
	std::string									msg;

	// _global_server에는 SERVER에 대한 내용만 있음, Client는 global_user에서 찾아야 함
	// 제거해야 할 fd를 가진 user들을 찾아서 quit
	while (begin != end)
	{
		if (begin->second->get_socket()->get_fd() == fd)
		{
			// SQUIT 전송?
			// + 연결된 다른 클라이언트들도 제거해야 함
			msg = ":" + irc.get_serverinfo().SERVER_NAME + " SQUIT " + begin->first + " :connect close\n";
			tmp = ++begin;
			--begin;
			irc.delete_server(begin->first);
			irc.send_msg_server(fd, msg.c_str());
			begin = tmp;
		}
		else if (begin != end)
			begin++;
	}
}

void	SquitCommand::delete_connected_server(IrcServer &irc, Server *server)
{
	if (server->get_hopcount() == 1) // 직접 연결된 서버라면
	{
		Socket *tmp = server->get_socket();
		irc.get_socket_set().remove_socket(server->get_socket()); // 1. 소켓셋에서 제거
		send_quit_user(server->get_socket()->get_fd(), irc);
		delete_server(server->get_socket()->get_fd(), irc); // 2. 다른곳에도 메세지 전달 + global_server에서 제거 + 소켓 인스턴스 제거 + 그쪽 서버에 연결된 유저 제거
		delete tmp;
	}
	else
	{
		irc.delete_server(server->get_name());
	}
}

void	SquitCommand::run(IrcServer &irc)
{
	Server				*server;
	Socket				*sock = irc.get_current_socket();
	std::string			servername;
	int					fd = sock->get_fd();

	// squit은 서버 혹은 관리자가 보냄
	if (sock->get_type() == UNKNOWN)
		throw (Reply(ERR::NOTREGISTERED()));
	else if (sock->get_type() == SERVER) // :servername SQUIT servername comment
	{
		if (_msg.get_param_size() == 0)
		{
			// 해당 서버와 연결이 끊겨서 메시지가 넘어오질 않은 경우
			irc.get_socket_set().remove_socket(sock);
			send_quit_user(fd, irc);
			delete_server(fd, irc);
			delete sock;
			return ;
		}
		if (_msg.get_param_size() != 2)
			throw (Reply(ERR::NEEDMOREPARAMS(), _msg.get_command()));
		if (_msg.get_prefix().empty()) // 직접 연결된 서버에서 SQUIT 하는 경우(signal처리로 들어오는 경우 or 다른 문제 때문에 보내는 경우)
		{
			// _global_server과 socketset에서 둘 다 지워야 함
			// 1. 해당 소켓 제거
			// 2. _global_server에서 제거(같은 fd를 가지는 서버도 제거 필요(해당 SQUIT를 전송하는 방향으로 처리))
			// 2. 다른 서버에 프리픽스 추가해서 SQUIT 전송
			irc.get_socket_set().remove_socket(sock);
			send_quit_user(fd, irc);
			delete_server(fd, irc);
			delete sock;
		}
		else // 다른 서버를 통해 연결되는 경우 (:test4001.com SQUIT test4007.com bye)
		{
			servername = _msg.get_param(0);
			if (servername == irc.get_serverinfo().SERVER_NAME)
			{
				server = irc.get_server(_msg.get_prefix());
				send_quit_user(server->get_socket()->get_fd(), irc);
				delete_server(server->get_socket()->get_fd(), irc);
				exit(0);
			}
			else
			{
				irc.delete_server(servername);
				irc.send_msg_server(fd, _msg.get_msg());
			}
		}
	}
	else // CLIENT
	{
		// 관리자가 아니라면 noprivilege error
		Member	*member = irc.find_member(sock->get_fd());

		if (_msg.get_param_size() != 2)
			throw (Reply(ERR::NEEDMOREPARAMS(), _msg.get_command()));
		if (member)
		{
			if (member->check_mode('o', true))
				throw (Reply(ERR::NOPRIVILEGES()));
			std::string		servername = _msg.get_param(0);

			if (servername.at(0) == ':')
				servername = servername.substr(1);
			Server	*server = irc.get_server(servername);

			if (!server)
				throw (Reply(ERR::NOSUCHSERVER(), servername));
			if (servername == irc.get_serverinfo().SERVER_NAME)
				throw (Reply(ERR::NOPRIVILEGES()));
			_msg.set_prefix(irc.get_serverinfo().SERVER_NAME);
			irc.send_msg_server(fd, _msg.get_msg());
			// 직접 연결 된 서버라면 여기서 제거해야 함
			delete_connected_server(irc, server); // 메세지 보낸 후에 제거?
		}
	}
}

SquitCommand::SquitCommand() : Command()
{
}

SquitCommand::~SquitCommand()
{
}