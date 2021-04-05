#ifndef TIMECOMMAND_HPP
# define TIMECOMMAND_HPP

# include "Command.hpp"

class TimeCommand: public Command
{
	public:
		TimeCommand();
		virtual ~TimeCommand();
		TimeCommand(TimeCommand const &ref);
		TimeCommand	&operator=(TimeCommand const &ref);
	public:
		void run(IrcServer &irc);
};

#endif