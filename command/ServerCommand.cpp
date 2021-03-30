#include "ServerCommand.hpp"
#include "ft_irc.hpp"

// PASS 등록이 안된 경우엔 해당 에러 메시지 반환만 함

void	ServerCommand::run(IrcServer &irc)
{
	SocketSet	&ss = irc.get_socket_set();
	Socket		*socket = irc.get_current_socket();


	if (irc.get_fd_map().find(_msg.get_param(0)) != irc.get_fd_map().end())
		throw (Reply(ERR::ALREADYREGISTRED()));
	if (socket->get_type() == UNKNOWN) // 새로운 서버 추가요청을 받은경우 (패스워드 확인 필요)
	{
		if (socket->get_pass().empty())
		{
			// :irc.example.net 461 * SERVER :Syntax error
			std::string msg = irc.get_servername();
			msg += " 461 * SERVER :Syntax error\n";
			socket->write(msg.c_str());
		}
		else if (irc.check_pass(socket))
		{
			// 1. 현재 fd의 소켓 타입 변경
			// 2. 이 메세지가 온 fd를 제외하고 && 모든 서버타입 소켓에 새로 등록된 SERVER 메세지 전송
			_msg.set_param_at(1, "0");
			irc.send_msg_server(socket->get_fd(), _msg.get_origin());
			ss.change_socket_type(_msg.get_source_fd(), SERVER);
			irc.add_fd_map(_msg.get_param(0).c_str(), socket->get_fd());
			irc.send_map_data(socket->get_fd());
		}
		else
		{
			irc.get_current_socket()->write("ERROR :Bad password\n");
		}
	}
	else if (socket->get_type() == CLIENT)
	{
		socket->write("ERROR :Server not configured here\n");
	}
	else if (socket->get_type() == SERVER) // 다른 서버에서 서버가 추가되었음을 알리는 경우 (패스워드 확인 필요 없음)
	{
		int hopcount = ft::atoi(_msg.get_param(1).c_str());
		hopcount++;
		_msg.set_param_at(1, std::to_string(hopcount));
		irc.send_msg_server(socket->get_fd(), _msg.get_origin());
		irc.add_fd_map(_msg.get_param(0).c_str(), socket->get_fd());
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