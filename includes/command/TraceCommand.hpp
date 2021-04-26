#ifndef TRACE_COMMAND_HPP
# define TRACE_COMMAND_HPP

# include "Command.hpp"

class TraceCommand : public Command
{
public:
	TraceCommand();
	~TraceCommand();
public:
	void	run(IrcServer &irc);
};

#endif