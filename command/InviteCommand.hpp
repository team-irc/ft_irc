#ifndef INVITE_COMMAND_HPP
# define INVITE_COMMAND_HPP

# include "Command.hpp"

class InviteCommand : public Command
{
private:

public:
	InviteCommand();
	virtual ~InviteCommand();
	InviteCommand(InviteCommand const &copy);
	InviteCommand	&operator=(InviteCommand const &ref);

	void	run(IrcServer &irc);
};

#endif