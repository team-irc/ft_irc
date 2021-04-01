#ifndef JOIN_COMMAND_HPP
# define JOIN_COMMAND_HPP

# include "Command.hpp"

class JoinCommand : public Command
{
private:

public:
	JoinCommand();
	virtual ~JoinCommand();
	JoinCommand(JoinCommand const &copy);
	JoinCommand	&operator=(JoinCommand const &ref);

	void	run(IrcServer &irc);
};

#endif