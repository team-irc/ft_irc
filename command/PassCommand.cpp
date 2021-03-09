#include "PassCommand.hpp"
#include "ft_irc.hpp"

void	PassCommand::run(IrcServer &irc)
{
	
	std::cout << "PassCommand executed.\n";
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