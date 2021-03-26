#ifndef NAMES_COMMAND_HPP
# define NAMES_COMMAND_HPP

#include "Command.hpp"
#include "Socket.hpp"
#include "Channel.hpp"

class NamesCommand: public Command
{
public:
	NamesCommand();
public:
	void run(IrcServer &irc);
private:
	std::string get_channel_user_list(Channel * channel);
};

#endif