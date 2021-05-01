#include "PongCommand.hpp"

PongCommand::PongCommand()
{}

PongCommand::~PongCommand()
{}

void			PongCommand::run(IrcServer &irc)
{
	Socket	*socket = irc.get_current_socket();
	Server	*server;

	if (socket->get_type() == UNKNOWN)
		throw (Reply(ERR::NOTREGISTERED()));
	else if (socket->get_type() == CLIENT)
	{
		if (_msg.get_param_size() == 0)
			throw(Reply(ERR::NOORIGIN()));
	}
	else if (socket->get_type() == SERVER)
	{
		std::string		servername;

		if (_msg.get_param_size() == 0)
			throw(Reply(ERR::NOORIGIN()));
		else if (_msg.get_param_size() == 1)
		{
			// 1개인 경우, server1은 해당 pong메시지르 ㄹ만든 주체
			servername = _msg.get_param(0);
			if (servername.at(0) == ':')
				servername = servername.substr(1);
			server = irc.get_server(servername);
			if (!server && servername != irc.get_serverinfo().SERVER_NAME)
				throw(Reply(ERR::NOSUCHSERVER(), servername));
		}
		else
		{
			// 2개인 경우, server1은 해당 pong을 만든 주체가 되고, server2로 전달해야 함
			servername = _msg.get_param(1);
			server = irc.get_server(servername);
		}
		if (servername.at(0) == ':')
			servername = servername.substr(1);
		if (servername != irc.get_serverinfo().SERVER_NAME)
		{
			if (!server)
			{
				// server가 아닌 경우(member)
				Member	*member = irc.get_member(servername);
				if (!member)
					throw(Reply(ERR::NOSUCHSERVER(), servername));
				if (member->get_socket()->get_type() == CLIENT)
					_msg.set_param_at(0, _msg.get_prefix());
				member->get_socket()->write(_msg.get_msg());
			}
			else
				server->get_socket()->write(_msg.get_msg());
		}
	}
}
