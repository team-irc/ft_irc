#include "ErrorCommand.hpp"

ErrorCommand::ErrorCommand(): Command()
{}

ErrorCommand::~ErrorCommand()
{}

/*
** ERROR :Server *.fi already exists; ERROR message to the other server
**										which caused this error.
** NOTICE WiZ :ERROR from csd.bu.edu -- Server *.fi already exists
**										; Same ERROR message as above but sent
**										to user WiZ on the other server.
*/

static Server	*find_server(IrcServer &irc, Socket *socket)
{
	std::map<std::string, Server *>::iterator	begin = irc.get_global_server().begin();
	std::map<std::string, Server *>::iterator	end = irc.get_global_server().end();
	while (begin != end)
	{
		if (begin->second->get_socket() == socket)
		{
			return (begin->second);
		}
		begin++;
	}
	return (NULL);
}

void            ErrorCommand::run(IrcServer &irc)
{
	Socket	*socket = irc.get_current_socket();

	if (socket->get_type() == SERVER)
	{
		Server			*server = find_server(irc, socket);
		std::string		msg = "NOTICE nick :ERROR from ";
		if (server)
			msg += server->get_name();
		else
			msg += socket->get_hostname();
		msg += " -- " + _msg.get_param(0) + "\n";
		Message message(msg.c_str());
		std::map<std::string, Member *>::iterator	begin = irc.get_global_user().begin();
		std::map<std::string, Member *>::iterator	end = irc.get_global_user().end();

		while (begin != end)
		{
			if (begin->second->check_mode('o', false))
			{
				message.set_param_at(0, begin->second->get_nick());
				begin->second->get_socket()->write(message.get_msg());
			}
			begin++;
		}
	}
	else if (socket->get_type() == UNKNOWN)
		throw (Reply(ERR::NOTREGISTERED()));
}
