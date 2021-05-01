#ifndef QUIT_COMMAND_HPP
# define QUIT_COMMAND_HPP

# include "Command.hpp"

class QuitCommand : public Command
{
private:

public:
	QuitCommand();
	~QuitCommand();
public:
	void	run(IrcServer &irc);
};

#endif