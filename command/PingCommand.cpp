#include "PingCommand.hpp"
#include "ft_irc.hpp"

PingCommand::PingCommand()
{}

PingCommand::~PingCommand()
{}

PingCommand::PingCommand(PingCommand const &ref)
{
	_msg = ref._msg;
}

PingCommand	&PingCommand::operator=(PingCommand const &ref)
{
	if (this != &ref)
		_msg = ref._msg;
	return (*this);
}

void		PingCommand::run(IrcServer &irc)
{
	Socket	*socket = irc.get_current_socket();

	if (socket->get_type() == UNKNOWN)
		throw (Reply(ERR::NOTREGISTERED()));
	else if (socket->get_type() == CLIENT)
	{
		// client
		// get_param이 2라면 해당 서버 검증 이후 ping 메시지 전송
		// 아니면 PONG을 해당 멤버에게 반환
		if (_msg.get_param_size() == 0)
			throw (Reply(ERR::NOORIGIN()));
		else if (_msg.get_param_size() == 1)
		{
			// server1만 설정된 상태
			// :servername PONG servername :server1
			std::string	param = _msg.get_param(0);
			if (param.at(0) == ':')
				param.substr(1);
			std::string	msg = ":" + irc.get_serverinfo().SERVER_NAME + " PONG " +
				irc.get_serverinfo().SERVER_NAME + " :" + param;
		}
		else
		{
			// server2가 설정 됨
			// 해당 서버로 PING 메세지 전송
		}
	}
	else
	{
		// server
		// 연결된 클라이언트들에 PING 메시지 전송
		// :prefix PING server2 :server1 or :prefix PING server1 server2
		// server2가 설정된 경우에는 server2로만 PING 메시지 전송
		// server2가 없으면 NOSUCHSERVER
		// server1, server2 없으면 noorigin
	}
	
}