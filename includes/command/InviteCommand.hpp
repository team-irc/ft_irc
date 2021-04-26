#ifndef INVITE_COMMAND_HPP
# define INVITE_COMMAND_HPP

# include "Command.hpp"

class InviteCommand : public Command
{
private:

public:
	InviteCommand();
	~InviteCommand();
public:
	void	run(IrcServer &irc);
};

#endif