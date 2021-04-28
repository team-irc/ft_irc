#ifndef NJOIN_COMMAND_HPP
# define NJOIN_COMMAND_HPP
# include "Command.hpp"

class NjoinCommand : public Command
{
public:
	NjoinCommand();
	~NjoinCommand();
	void	run(IrcServer &irc);
};

#endif