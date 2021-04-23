#ifndef JOIN_COMMAND_HPP
# define JOIN_COMMAND_HPP

# include "Command.hpp"

class JoinCommand : public Command
{
private:
	int		join(IrcServer &irc, Member *member, std::string const &channel_name, std::string const &channel_key);

public:
	JoinCommand();
	virtual ~JoinCommand();
	JoinCommand(JoinCommand const &copy);
	JoinCommand	&operator=(JoinCommand const &ref);

	void	run(IrcServer &irc);
};

#endif