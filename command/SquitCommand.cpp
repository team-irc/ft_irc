#include "SquitCommand.hpp"
#include "ft_irc.hpp"

void	SquitCommand::run(IrcServer &irc)
{
	Socket	*sock = irc.get_current_socket();
	unsigned short		port;
	int					fd = irc.get_current_socket()->get_fd();
	std::string			msg = "";

	// SQUIT은 서버에서 보내는 메시지라서 다른 타입이면 걍 무시
	if (sock->get_type() == SERVER)
	{
		if (_msg.get_prefix().empty())
		{
			port = irc.get_current_socket()->get_port();
			// 이 경우는 직접 연결된 서버에서 연결 종료 메시지를 보낸 경우
			// fd_map과 socketset에서 둘 다 지워야 함
			irc.delete_fd_map(port);
			irc.get_socket_set().remove_socket(irc.get_current_socket());
			msg += ":" + std::to_string(port) + " " + _msg.get_msg();
		}
		else
		{
			// 이 경우는 그냥 전달받은 경우
			// fd_map에서 해당 포트 번호에 해당하는 값을 지우면 됨
			port = ft::atoi(_msg.get_prefix().c_str());
			irc.delete_fd_map(port);
		}
		irc.send_msg_server(fd, msg.c_str());
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