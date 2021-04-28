#include "NjoinCommand.hpp"

void	NjoinCommand::run(IrcServer &irc)
{
	Socket		*socket;
	Channel		*channel;
	int			i;
	int			size;
	std::string	*s_members;
	std::string	nick;
	Member		*member;

	socket = irc.get_current_socket();
	if (socket->get_type() == CLIENT) // 클라이언트에서 온 경우 무시
	{
		return ;
	}
	else if (socket->get_type() == SERVER)
	{
		// :servername NJOIN channel members
		// 1 채널생성
		// 2 채널에 멤버 추가
		// 3 전달
		channel = new Channel(_msg.get_param(0));
		irc.add_channel(_msg.get_param(0), channel);
		size = ft::split(_msg.get_param(1), ',', s_members);
		i = 0;
		while (i < size)
		{
			if (s_members[i].at(0) == '@')
			{
				if (s_members[i].at(1) == '@')
				{
					nick = s_members[i].substr(2, s_members[i].size());
					member = irc.get_member(nick);
					member->add_channel(channel);
					channel->add_member(member);
					channel->add_creator(member);
					channel->add_operator(member);
				}
				else
				{
					nick = s_members[i].substr(1, s_members[i].size());
					member = irc.get_member(nick);
					member->add_channel(channel);
					channel->add_member(member);
					channel->add_operator(member);
				}
			}
			else if (s_members[i].at(0) == '+')
			{
				nick = s_members[i].substr(1, s_members[i].size());
				member = irc.get_member(nick);
				member->add_channel(channel);
				channel->add_member(member);
				channel->add_voice(member);
			}
			else
			{
				channel->add_member(irc.get_member(s_members[i]));
				member->add_channel(channel);
			}
			i++;
		}
		irc.send_msg_server(socket->get_fd(), _msg.get_msg());
	}
	else if (socket->get_type() == UNKNOWN)
		throw (Reply(ERR::NOTREGISTERED()));
}

NjoinCommand::NjoinCommand() : Command()
{
}

NjoinCommand::~NjoinCommand()
{
}