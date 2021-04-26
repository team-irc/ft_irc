#ifndef JOIN_COMMAND_HPP
# define JOIN_COMMAND_HPP

# include "Command.hpp"

class JoinCommand : public Command
{
private:
	int		join(IrcServer &irc, Member *member, std::string const &channel_name, std::string const &channel_key);

public:
	JoinCommand();
	~JoinCommand();
public:
	void	run(IrcServer &irc);
};

#endif