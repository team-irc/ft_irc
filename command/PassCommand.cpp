#include "PassCommand.hpp"
#include "ft_irc.hpp"

void	PassCommand::run(IrcServer &irc)
{
	Member	*member = irc.find_member(_msg.get_source_fd());
	Socket	*sock = irc.get_current_socket();
	
	if (member)
		sock->write(":servername 462 *(or nick) :Connection already registered\n");
	else if (!irc.get_input_pass().empty())
		sock->write(":servername 462 *(or nick) :Connection already registered\n");
	else
		_input_pass = _msg.get_param(0);
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