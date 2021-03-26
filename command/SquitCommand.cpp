#include "SquitCommand.hpp"
#include "ft_irc.hpp"

void	SquitCommand::run(IrcServer &irc)
{
}

SquitCommand::SquitCommand() : Command()
{
}

SquitCommand::~SquitCommand()
{
}

SquitCommand::SquitCommand(SquitCommand const &copy)
{
	_msg = copy._msg;
}

SquitCommand	&SquitCommand::operator=(SquitCommand const &ref)
{
	_msg = ref._msg;
	return (*this);
}