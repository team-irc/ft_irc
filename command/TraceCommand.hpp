#ifndef TRACE_COMMAND_HPP
# define TRACE_COMMAND_HPP

# include "Command.hpp"

class TraceCommand : public Command
{
private:

public:
	TraceCommand();
	virtual ~TraceCommand();
	TraceCommand(TraceCommand const &copy);
	TraceCommand	&operator=(TraceCommand const &ref);

	void	run(IrcServer &irc);
};

#endif