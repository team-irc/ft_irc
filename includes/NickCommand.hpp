#ifndef NICK_COMMAND_HPP
# define NICK_COMMAND_HPP

# include "Command.hpp"

class NickCommand : public Command
{
public:
	NickCommand();
	virtual ~NickCommand();
	NickCommand(NickCommand const &copy);
	NickCommand	&operator=(NickCommand const &ref);
	void	run(IrcServer &irc);
private:
	bool check_nick(const std::string &nick);
};

#endif