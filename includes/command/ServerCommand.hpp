#ifndef SERVER_COMMAND_HPP
# define SERVER_COMMAND_HPP

# include "Command.hpp"
# include <map>

class ServerCommand : public Command
{
private:

public:
	ServerCommand();
	~ServerCommand();
public:
	void	run(IrcServer &irc);
};

#endif