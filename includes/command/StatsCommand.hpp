#ifndef STATS_COMMAND_HPP
# define STATS_COMMAND_HPP

# include "Command.hpp"

class StatsCommand : public Command
{
private:

public:
	StatsCommand();
	virtual ~StatsCommand();
	StatsCommand(StatsCommand const &copy);
	StatsCommand	&operator=(StatsCommand const &ref);

	void	run(IrcServer &irc);
	void	stats(IrcServer &irc, Socket *socket, char flag);
};

#endif