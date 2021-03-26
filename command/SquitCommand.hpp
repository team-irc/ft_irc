#ifndef SQUIT_COMMAND_HPP
# define SQUIT_COMMAND_HPP

# include "Command.hpp"


class SquitCommand : public Command
{
private:

public:
	SquitCommand();
	virtual ~SquitCommand();
	SquitCommand(SquitCommand const &copy);
	SquitCommand	&operator=(SquitCommand const &ref);

	void	run(IrcServer &irc);
};

#endif