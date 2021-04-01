#ifndef KICK_COMMAND_HPP
# define KICK_COMMAND_HPP

#include "Command.hpp"
#include "Channel.hpp"
#include "Member.hpp"

class KickCommand: public Command
{
public:
	KickCommand();
	~KickCommand();
public:
	void run(IrcServer &irc);
private:
	void kick_notification(const std::string &, Channel *, IrcServer &);
};

#endif