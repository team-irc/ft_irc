#include "NamesCommand.hpp"

void	NamesCommand::run(IrcServer &irc)
{
	Socket	*socket;

	socket = irc.get_current_socket();
}

NamesCommand::NamesCommand(): Command()
{
}