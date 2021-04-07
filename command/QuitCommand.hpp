#ifndef QUIT_COMMAND_HPP
# define QUIT_COMMAND_HPP

# include "Command.hpp"

class QuitCommand : public Command
{
private:

public:
	QuitCommand();
	virtual ~QuitCommand();
	QuitCommand(QuitCommand const &copy);
	QuitCommand	&operator=(QuitCommand const &ref);

	void	run(IrcServer &irc);
};

#endif