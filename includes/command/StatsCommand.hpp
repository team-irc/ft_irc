#ifndef STATS_COMMAND_HPP
# define STATS_COMMAND_HPP

# include "Command.hpp"

class StatsCommand : public Command
{
public:
	StatsCommand();
	~StatsCommand();
public:
	void	run(IrcServer &irc);
private:
	void	stats(IrcServer &irc, Socket *socket, char flag);
};

#endif