#include "JoinCommand.hpp"
#include "ft_irc.hpp"

void	JoinCommand::run(IrcServer &irc)
{
	Channel
	std::cout << "Join Command executed.\n";
}

JoinCommand::JoinCommand() : Command()
{
}

JoinCommand::~JoinCommand()
{
}

JoinCommand::JoinCommand(JoinCommand const &copy)
{
	_msg = copy._msg;
}

JoinCommand	&JoinCommand::operator=(JoinCommand const &ref)
{
	_msg = ref._msg;
	return (*this);
}