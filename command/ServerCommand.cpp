#include "ServerCommand.hpp"
#include "ft_irc.hpp"

void	ServerCommand::run(IrcServer &irc)
{
	SocketSet	&ss = irc.get_socket_set();

	// 1. 현재 fd의 소켓 타입 변경
	ss.change_socket_type(_msg.get_source_fd(), SERVER);
	// 2. 이 메세지가 온 fd를 제외하고 && 모든 서버타입 소켓에 새로 등록된 SERVER 메세지 전송
	for (int i = 3; i <= irc.get_fdmax(); i++)
	{
		if (ss.find_socket(i)->get_type() == SERVER && i != _msg.get_source_fd())
		{
			
			irc.send_msg(i, _msg.get_origin());
			
			std::cout << "Message send\n"; //
			std::cout << "to: " << i << " | " << _msg.get_origin() << std::endl;
		}
	}
	std::cout << "SERVER command executed.\n";
}

ServerCommand::ServerCommand() : Command()
{
}

ServerCommand::~ServerCommand()
{
}

ServerCommand::ServerCommand(ServerCommand const &copy)
{
	_msg = copy._msg;
}

ServerCommand	&ServerCommand::operator=(ServerCommand const &ref)
{
	_msg = ref._msg;
	return (*this);
}