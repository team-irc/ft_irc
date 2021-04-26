#ifndef TIMECOMMAND_HPP
# define TIMECOMMAND_HPP

# include "Command.hpp"

class TimeCommand: public Command
{
public:
	TimeCommand();
	~TimeCommand();
public:
	void run(IrcServer &irc);
};

#endif