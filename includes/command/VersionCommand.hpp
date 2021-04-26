#ifndef VERSION_COMMAND_HPP
# define VERSION_COMMAND_HPP

# include "Command.hpp"

class VersionCommand : public Command
{
public:
	VersionCommand();
	~VersionCommand();
public:
	void	run(IrcServer &irc);
};

#endif