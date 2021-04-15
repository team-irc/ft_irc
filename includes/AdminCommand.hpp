#ifndef ADMINCOMMAND_HPP
# define ADMINCOMMAND_HPP

# include "Command.hpp"

class	AdminCommand : public Command
{
	public:
		AdminCommand();
		virtual ~AdminCommand();
		AdminCommand(AdminCommand const &ref);
		AdminCommand	&operator=(AdminCommand const &ref);
	public:
		void			run(IrcServer &irc);
};

#endif