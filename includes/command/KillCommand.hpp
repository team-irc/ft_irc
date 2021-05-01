#ifndef KILL_COMMAND_HPP
# define KILL_COMMAND_HPP

# include "Command.hpp"

class KillCommand : public Command
{
private:

public:
	KillCommand();
	~KillCommand();
public:
	void	run(IrcServer &irc);
};

#endif