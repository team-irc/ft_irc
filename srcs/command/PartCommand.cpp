#include "PartCommand.hpp"

void	PartCommand::run(IrcServer &irc)
{
	int				size;
	std::string		*channel_names;
	Channel			*channel;
	Socket			*socket;
	Member			*member;

	socket = irc.get_current_socket();
	if (socket->get_type() == CLIENT) // 클라이언트에서 온 경우
	{
		if (_msg.get_param_size() < 1)
			throw (Reply(ERR::NEEDMOREPARAMS(), "PART"));
		size = ft::split(_msg.get_param(0), ',', channel_names);
		member = irc.find_member(socket->get_fd());
		for (int i = 0; i < size; i++)
		{
			channel = irc.get_channel(channel_names[i]);
			if (channel == NULL)
				throw (Reply(ERR::NOSUCHCHANNEL(), channel_names[i]));
			if (!channel->find_member(member))
				throw (Reply(ERR::NOTONCHANNEL(), channel_names[i]));
			// 서버에 전송
			_msg.set_prefix(member->get_nick());
			_msg.set_param_at(0, channel_names[i]);
			irc.send_msg_server(socket->get_fd(), _msg.get_msg());
			// 클라이언트에 전송
			_msg.set_prefix(member->get_nick() + "~!" + member->get_username() + "@" + member->get_hostname());
			channel->send_msg_to_members(_msg.get_msg());
			
			channel->delete_member(member);
			member->delete_channel(channel);
			if (channel->get_members().empty()) // 채널에 아무도 없다면 채널 삭제
			{
				irc.delete_channel(channel->get_name());
				delete channel;
			}
		}
		delete[] channel_names;
	}
	else if (socket->get_type() == SERVER)
	{
		size = ft::split(_msg.get_param(0), ',', channel_names);
		std::string	nickname = _msg.get_prefix();
		member = irc.get_member(nickname);
		for (int i = 0; i < size; i++)
		{
			channel = irc.get_channel(channel_names[i]);
			irc.send_msg_server(socket->get_fd(), _msg.get_msg());
			channel->send_msg_to_members(_msg.get_msg());
			channel->delete_member(member);
			member->delete_channel(channel);
			if (channel->get_members().empty()) // 채널에 아무도 없다면 채널 삭제
			{
				irc.delete_channel(channel->get_name());
				delete channel;
			}
		}
	}
	else if (socket->get_type() == UNKNOWN)
		throw (Reply(ERR::NOTREGISTERED()));
}

PartCommand::PartCommand() : Command()
{
}

PartCommand::~PartCommand()
{
}