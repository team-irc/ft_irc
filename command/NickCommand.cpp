#include "NickCommand.hpp"
#include "ft_irc.hpp"

void	NickCommand::run(IrcServer &irc)
{
	
	std::cout << "Nick Command executed.\n";
}

NickCommand::NickCommand() : Command()
{
}

NickCommand::~NickCommand()
{
}

NickCommand::NickCommand(NickCommand const &copy)
{
	_msg = copy._msg;
}

NickCommand	&NickCommand::operator=(NickCommand const &ref)
{
	_msg = ref._msg;
	return (*this);
}