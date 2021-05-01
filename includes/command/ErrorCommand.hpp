#ifndef ERRORCOMMAND_HPP
# define ERRORCOMMAND_HPP

# include "Command.hpp"

class	ErrorCommand : public Command
{
	public:
		ErrorCommand();
		~ErrorCommand();
	public:
		void			run(IrcServer &irc);
};

#endif