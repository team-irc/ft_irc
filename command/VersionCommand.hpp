#ifndef VERSION_COMMAND_HPP
# define VERSION_COMMAND_HPP

# include "Command.hpp"

class VersionCommand : public Command
{
private:

public:
	VersionCommand();
	virtual ~VersionCommand();
	VersionCommand(VersionCommand const &copy);
	VersionCommand	&operator=(VersionCommand const &ref);

	void	run(IrcServer &irc);
};

#endif