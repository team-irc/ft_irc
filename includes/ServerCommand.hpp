#ifndef SERVER_COMMAND_HPP
# define SERVER_COMMAND_HPP

# include "Command.hpp"
# include <map>

class ServerCommand : public Command
{
private:

public:
	ServerCommand();
	virtual ~ServerCommand();
	ServerCommand(ServerCommand const &copy);
	ServerCommand	&operator=(ServerCommand const &ref);
	void	run(IrcServer &irc);
};

#endif