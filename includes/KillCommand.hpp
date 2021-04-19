#ifndef KILLCOMMAND_HPP
# define KILLCOMMAND_HPP

# include "Command.hpp"

class	KillCommand : public Command
{
	public:
		KillCommand();
		virtual ~KillCommand();
		KillCommand(KillCommand const &ref);
		KillCommand	&operator=(KillCommand const &ref);
	public:
		void			run(IrcServer &irc);
};

#endif