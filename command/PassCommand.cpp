#include "PassCommand.hpp"
#include "ft_irc.hpp"

void	PassCommand::run(IrcServer &irc)
{
	Member	*member = irc.find_member(_msg.get_source_fd());
	Socket	*sock = irc.get_current_socket();
	
	if (member)
		sock->write(Reply(ERR::ALREADYREGISTRED()).get_msg().c_str());
	else if (_msg.get_param_size() <= 0)
		sock->write(Reply(ERR::NEEDMOREPARAMS(), _msg.get_command()).get_msg().c_str());
	else
	{
		sock->set_pass(_msg.get_param(0));
	}
}

PassCommand::PassCommand() : Command()
{
}

PassCommand::~PassCommand()
{
}

PassCommand::PassCommand(PassCommand const &copy)
{
	_msg = copy._msg;
}

PassCommand	&PassCommand::operator=(PassCommand const &ref)
{
	_msg = ref._msg;
	return (*this);
}