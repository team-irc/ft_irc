#ifndef NOTICE_COMMAND_HPP
# define NOTICE_COMMAND_HPP
# include "Command.hpp"

class Channel;
class Member;

class NoticeCommand : public Command
{
public:
	NoticeCommand();
	~NoticeCommand();
	void	run(IrcServer &irc);
private:
	void	add_prefix(IrcServer &irc);
	void	check_receiver(IrcServer &irc, const std::string &recv,
		std::vector<std::string> &send_nick, std::vector<int> &send_channel_fd);
	void	send_channel(IrcServer &irc, Channel &channel, Member *sender, std::vector<int> &send_channel_fd);
	void	send_member(IrcServer &irc, Member &member, std::vector<std::string> &send_member);
};

#endif