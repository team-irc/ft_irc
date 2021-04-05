#ifndef NOTICE_COMMAND_HPP
# define NOTICE_COMMAND_HPP
# include "Command.hpp"

class Channel;
class Member;

class NoticeCommand : public Command
{
public:
	NoticeCommand();
	NoticeCommand(const NoticeCommand &ref);
	NoticeCommand	&operator=(const NoticeCommand &ref);
	virtual ~NoticeCommand();
	void	run(IrcServer &irc);
private:
	void	add_prefix(IrcServer &irc);
	void	check_receiver(IrcServer &irc, const std::string &recv);
	void	send_channel(IrcServer &irc, Channel &channel);
	void	send_member(IrcServer &irc, Member &member);
};

#endif