#ifndef USER_COMMAND_HPP
# define USER_COMMAND_HPP

# include "Command.hpp"

class Member;

class UserCommand : public Command
{
private:
	void  insert_info(Member *member, IrcServer &irc);
public:
	UserCommand();
	virtual ~UserCommand();
	UserCommand(UserCommand const &copy);
	UserCommand	&operator=(UserCommand const &ref);
	void	run(IrcServer &irc);
};

#endif