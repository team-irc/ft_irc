#ifndef NAMES_COMMAND_HPP
# define NAMES_COMMAND_HPP

#include "Command.hpp"
#include "Socket.hpp"

class NamesCommand: public Command
{
public:
	NamesCommand();
public:
	void run(IrcServer &irc);
};

#endif