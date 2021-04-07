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
			// :servername PONG servername :server1 형태의 메시지를 클라이언트에 전송
			std::string	param = _msg.get_param(0);
			if (param.at(0) == ':')
				param.substr(1);
			std::string	msg = ":" + irc.get_serverinfo().SERVER_NAME + " PONG " +
				irc.get_serverinfo().SERVER_NAME + " :" + param;
			socket->write(msg.c_str());
		}
		else
		{
			// server2가 설정 됨
			// 해당 서버로 PING 메세지 전송
			// 1차로 서버 검증 과정 거침
			Server	*server;
			if ((server = irc.get_server(_msg.get_param(1))) == 0)
				throw(Reply(ERR::NOSUCHSERVER(), _msg.get_param(1)));
			_msg.set_prefix(irc.get_serverinfo().SERVER_NAME);
			server->get_socket()->write(_msg.get_msg());
		}
	}
	else
	{
		std::string		servername = _msg.get_param(_msg.get_param_size() - 1);

		if (servername.at(0) == ':')
			servername = servername.substr(1);
		if (_msg.get_param_size() == 1)
		{
			//자신에게 연결된 클라이언트에게 저 메시지를 전송하면 됨
			std::vector<Socket *> sockets = irc.get_socket_set().get_connect_sockets();
			std::vector<Socket *>::iterator	begin = sockets.begin();
			std::vector<Socket *>::iterator	end = sockets.end();

			while (begin != end)
			{
				if ((*begin)->get_type() == CLIENT)
					(*begin)->write(_msg.get_msg());
				begin++;
			}
		}
		// 아니면 해당 서버가 받을 수 있도록 전송
		// :prefix PING server1 :server2 형태
		else if (irc.get_serverinfo().SERVER_NAME == servername)
		{
			// server2가 자신의 서버라면 연결된 클라이언트들에 PING 메시지 전송
			std::vector<Socket *> sockets = irc.get_socket_set().get_connect_sockets();
			std::vector<Socket *>::iterator	begin = sockets.begin();
			std::vector<Socket *>::iterator	end = sockets.end();

			while (begin != end)
			{
				if ((*begin)->get_type() == CLIENT)
					(*begin)->write(_msg.get_msg());
				begin++;
			}
		}
		else
			irc.get_server(servername)->get_socket()->write(_msg.get_msg());
	}
	
}