#ifndef PINGCOMMAND_HPP
# define PINGCOMMAND_HPP

# include "Command.hpp"

class PingCommand : public Command
{
	public:
		PingCommand();
		virtual ~PingCommand();
		PingCommand(PingCommand const &ref);
		PingCommand	&operator=(PingCommand const &ref);
	public:
		void		run(IrcServer &irc);
};

#endif