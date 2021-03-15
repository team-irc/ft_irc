#include "UserCommand.hpp"
#include "ft_irc.hpp"

void	UserCommand::run(IrcServer &irc)
{
	
	cout << "User command executed.\n";
}

UserCommand::UserCommand() : Command()
{
}

UserCommand::~UserCommand()
{
}

UserCommand::UserCommand(UserCommand const &copy)
{
	_msg = copy._msg;
}

UserCommand	&UserCommand::operator=(UserCommand const &ref)
{
	_msg = ref._msg;
	return (*this);
}