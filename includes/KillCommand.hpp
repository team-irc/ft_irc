#ifndef KILL_COMMAND_HPP
# define KILL_COMMAND_HPP

# include "Command.hpp"

class KillCommand : public Command
{
private:

public:
	KillCommand();
	virtual ~KillCommand();
	KillCommand(KillCommand const &copy);
	KillCommand	&operator=(KillCommand const &ref);

	void	run(IrcServer &irc);
};

#endif