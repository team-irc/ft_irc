#include "TopicCommand.hpp"
#include "ft_irc.hpp"

void	TopicCommand::run(IrcServer &irc)
{
	
}

TopicCommand::TopicCommand() : Command()
{
}

TopicCommand::~TopicCommand()
{
}

TopicCommand::TopicCommand(TopicCommand const &copy)
{
	_msg = copy._msg;
}

TopicCommand	&TopicCommand::operator=(TopicCommand const &ref)
{
	_msg = ref._msg;
	return (*this);
}