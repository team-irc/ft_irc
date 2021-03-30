#include "PassCommand.hpp"
#include "ft_irc.hpp"

void	PassCommand::run(IrcServer &irc)
{
	Member	*member = irc.find_member(_msg.get_source_fd());
	Socket	*sock = irc.get_current_socket();

	if (!deal_exception(irc))
		return ;
	sock->set_pass(_msg.get_param(0));
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

bool PassCommand::deal_exception(IrcServer &irc)
{
	Member	*member = irc.find_member(_msg.get_source_fd());
	Socket	*sock = irc.get_current_socket();

	if (_msg.get_param_size() <= 0)
		goto ERR_NEEDMOREPARAMS;
	if (member)
		goto ERR_ALREADYREGISTRED;
	return (true);

ERR_NEEDMOREPARAMS:
	sock->write(Reply(ERR::NEEDMOREPARAMS(), _msg.get_command()).get_msg().c_str());
	return (false);

ERR_ALREADYREGISTRED:
	sock->write(Reply(ERR::ALREADYREGISTRED()).get_msg().c_str());
	return (false);
}