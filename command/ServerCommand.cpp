#include "ServerCommand.hpp"
#include "ft_irc.hpp"


// PASS 등록이 안된 경우엔 해당 에러 메시지 반환만 함

void	ServerCommand::run(IrcServer &irc)
{
	SocketSet	&ss = irc.get_socket_set();
	Socket		*socket = irc.get_current_socket();
	
	if (irc.get_input_pass().empty())
	{
		// :irc.example.net 461 * SERVER :Syntax error
		irc.get_current_socket()->write(":servername 461 * SERVER :Syntax error\n");
	}
	else if (irc.check_pass())
	{
		// 1. 현재 fd의 소켓 타입 변경
		ss.change_socket_type(_msg.get_source_fd(), SERVER);

		// 2. 이 메세지가 온 fd를 제외하고 && 모든 서버타입 소켓에 새로 등록된 SERVER 메세지 전송
		irc.send_msg_server(socket->get_fd(), _msg.get_origin());

		irc.add_fd_map(_msg.get_param(0).c_str(), socket->get_fd());
		std::cout << "SERVER command executed.\n";
	}
	else
	{
		irc.get_current_socket()->write("ERROR :Bad password\n");
	}
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