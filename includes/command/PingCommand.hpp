#ifndef PINGCOMMAND_HPP
# define PINGCOMMAND_HPP

# include "Command.hpp"

class PingCommand : public Command
{
public:
	PingCommand();
	~PingCommand();
public:
	void		run(IrcServer &irc);
};

#endif