#ifndef LIST_COMMAND_HPP
# define LIST_COMMAND_HPP

# include "Command.hpp"

class ListCommand: public Command
{
public:
	ListCommand();
public:
	void run(IrcServer &irc);
};

#endif