#ifndef ERRORCOMMAND_HPP
# define ERRORCOMMAND_HPP

# include "Command.hpp"

class	ErrorCommand : public Command
{
	public:
		ErrorCommand();
		virtual ~ErrorCommand();
		ErrorCommand(ErrorCommand const &ref);
		ErrorCommand	&operator=(ErrorCommand const &ref);
	public:
		void			run(IrcServer &irc);
};

#endif