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
	~UserCommand();
public:
	void	run(IrcServer &irc);
};

#endif