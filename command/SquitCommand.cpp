#include "SquitCommand.hpp"
#include "ft_irc.hpp"

void	SquitCommand::send_quit_user(int fd, IrcServer &irc)
{
	Member			*member;
	std::string		msg;

	while ((member = irc.find_member(fd)))
	{
		msg = ":" + member->get_nick() + " QUIT :server connect out\n";
		irc.send_msg_server(fd, msg.c_str());
		irc.delete_member(member->get_nick());
		delete (member);
	}
}

void	SquitCommand::delete_fd_map(int fd, IrcServer &irc)
{
	std::map<std::string, int>::iterator	begin = irc.get_fd_map().begin();
	std::map<std::string, int>::iterator	end = irc.get_fd_map().end();
	std::map<std::string, int>::iterator	tmp;
	std::string								msg;

	// fd_map에는 SERVER에 대한 내용만 있음, Client는 global_user에서 찾아야 함
	// 제거해야 할 fd를 가진 user들을 찾아서 quit
	while (begin != end)
	{
		if (begin->second == fd)
		{
			// SQUIT 전송?
			// + 연결된 다른 클라이언트들도 제거해야 함
			msg = ":" + irc.get_servername() + " SQUIT " + begin->first + " :connect close\n";
			tmp = ++begin;
			--begin;
			irc.delete_fd_map(begin->first);
			irc.send_msg_server(fd, msg.c_str());
			begin = tmp;
		}
		else if (begin != end)
			begin++;
	}
	send_quit_user(fd, irc);
}

void	SquitCommand::run(IrcServer &irc)
{
	Socket				*sock = irc.get_current_socket();
	std::string			servername;
	int					fd = sock->get_fd();

	// SQUIT은 서버에서 보내는 메시지라서 다른 타입이면 걍 무시
	if (sock->get_type() == SERVER)
	{
		if (_msg.get_prefix().empty()) // 직접 연결된 서버에서 SQUIT 하는 경우(signal처리로 들어오는 경우 or 다른 문제 때문에 보내는 경우)
		{
			// fd_map과 socketset에서 둘 다 지워야 함
			// 1. 해당 소켓 제거
			// 2. fd_map에서 제거(같은 fd를 가지는 서버도 제거 필요(해당 SQUIT를 전송하는 방향으로 처리))
			// 2. 다른 서버에 프리픽스 추가해서 SQUIT 전송
			irc.get_socket_set().remove_socket(sock);
			delete_fd_map(fd, irc);
			delete sock;
		}
		else // 다른 서버를 통해 연결되는 경우
		{
			servername = _msg.get_param(0);
			irc.delete_fd_map(servername);
			_msg.set_prefix(irc.get_servername());
			irc.send_msg_server(fd, _msg.get_msg());
		}
	}
}

SquitCommand::SquitCommand() : Command()
{
}

SquitCommand::~SquitCommand()
{
}

SquitCommand::SquitCommand(SquitCommand const &copy)
{
	_msg = copy._msg;
}

SquitCommand	&SquitCommand::operator=(SquitCommand const &ref)
{
	_msg = ref._msg;
	return (*this);
}