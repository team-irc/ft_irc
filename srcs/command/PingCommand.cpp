#include "PingCommand.hpp"

PingCommand::PingCommand(): Command()
{}

PingCommand::~PingCommand()
{}

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
				irc.get_serverinfo().SERVER_NAME + " :" + param + "\n";
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
			_msg.set_prefix(irc.find_member(socket->get_fd())->get_nick());
			if (server->get_socket()->get_type() == SERVER)
				server->get_socket()->write(_msg.get_msg());
			else
			{
				std::string	param = _msg.get_param(0);
				if (param.at(0) == ':')
					param.substr(1);
				std::string	msg = ":" + irc.get_serverinfo().SERVER_NAME + " PONG " +
				irc.get_serverinfo().SERVER_NAME + " :" + param + "\n";
				socket->write(msg.c_str());
			}
		}
	}
	else if (socket->get_type() == SERVER)
	{
		std::string		servername = _msg.get_param(_msg.get_param_size() - 1);

		if (servername.at(0) == ':')
			servername = servername.substr(1);
		if ((_msg.get_param_size() == 1) ||
			(irc.get_serverinfo().SERVER_NAME == servername))
		{
			//pong메시지 전송
			std::string		pong_msg = ":" + irc.get_serverinfo().SERVER_NAME + " PONG";
			if (!_msg.get_prefix().empty())
				pong_msg += " " + _msg.get_prefix();
			if (_msg.get_param(0).at(0) == ':')
				pong_msg += " " + _msg.get_param(0) + "\n";
			else
				pong_msg += " :" + _msg.get_param(0) + "\n";
			irc.get_socket_set().find_socket(_msg.get_source_fd())->write(pong_msg.c_str());
		}
		else
			irc.get_server(servername)->get_socket()->write(_msg.get_msg());
	}
	
}