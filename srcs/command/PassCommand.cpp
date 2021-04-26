#include "PassCommand.hpp"

void	PassCommand::run(IrcServer &irc)
{
	Member	*member = irc.find_member(_msg.get_source_fd());
	Socket	*sock = irc.get_current_socket();

	if (_msg.get_param_size() <= 0)
		throw (Reply(ERR::NEEDMOREPARAMS(), "PASS"));
	if (member)
		throw (Reply(ERR::ALREADYREGISTRED()));
	sock->set_pass(_msg.get_param(0));
}

PassCommand::PassCommand() : Command()
{
}

PassCommand::~PassCommand()
{
}