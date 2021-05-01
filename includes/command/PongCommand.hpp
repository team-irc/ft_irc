#ifndef PONGCOMMAND_HPP
# define PONGCOMMAND_HPP

# include "Command.hpp"

class PongCommand : public Command
{
public:
	PongCommand();
	~PongCommand();
public:
	void	run(IrcServer &irc);
};

#endif