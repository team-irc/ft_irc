#ifndef ADMINCOMMAND_HPP
# define ADMINCOMMAND_HPP

# include "Command.hpp"

class	AdminCommand : public Command
{
public:
	AdminCommand();
	~AdminCommand();
public:
	void			run(IrcServer &irc);
};

#endif