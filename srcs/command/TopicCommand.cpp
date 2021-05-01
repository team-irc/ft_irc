#include "TopicCommand.hpp"

void	TopicCommand::run(IrcServer &irc)
{
	Socket			*socket;
	Channel			*channel;
	Member			*member;
	std::string		channel_name;
	std::string		topic;

	socket = irc.get_current_socket();
	if (socket->get_type() == UNKNOWN)
		throw (Reply(ERR::NOTREGISTERED()));
	else if (socket->get_type() == CLIENT)
	{
		if (_msg.get_param_size() < 1)
			throw (Reply(ERR::NEEDMOREPARAMS(), _msg.get_command()));
		member = irc.find_member(socket->get_fd());
		channel_name = _msg.get_param(0);
		channel = irc.get_channel(channel_name);
		if (!channel) // 채널이 없다면 에러
			throw(Reply(ERR::NOSUCHCHANNEL(), channel_name));
		if (!channel->find_member(member)) // 맴버가 채널에 속해있지 않다면 에러
			throw(Reply(ERR::NOTONCHANNEL(), channel_name));
		if (_msg.get_param_size() == 1) // 1. 토픽 조회하는 경우
		{
			topic = channel->get_topic();
			if (topic.empty())
				throw(Reply(RPL::NOTOPIC(), channel_name));
			else
				socket->write(Reply(RPL::TOPIC(), channel_name, topic).get_msg().c_str());
		}
		else // 2. 토픽 설정하는 경우
		{
			if (channel->check_mode('t', false) == 1 && !channel->is_operator(member)) // 채널 모드가 i라면 오퍼레이터만 가능
				throw(Reply(ERR::CHANOPRIVSNEEDED(), channel_name));
			else
			{
				topic = _msg.get_param(1);
				if (topic.at(0) == ':')
					topic = topic.substr(1, topic.length() - 1);
				channel->set_topic(topic);
				if (_msg.get_prefix().empty())
					_msg.set_prefix(member->get_nick());
				irc.send_msg_server(socket->get_fd(), _msg.get_msg());
			}
		}
	}
	else if (socket->get_type() == SERVER)
	{
		// :nick TOPIC #test :hello world
		member = irc.get_member(_msg.get_prefix());
		channel_name = _msg.get_param(0);
		channel = irc.get_channel(channel_name);
		topic = _msg.get_param(1);
		if (topic.at(0) == ':')
			topic = topic.substr(1, topic.length() - 1);
		channel->set_topic(topic);
		irc.send_msg_server(socket->get_fd(), _msg.get_msg());
	}

	
}

TopicCommand::TopicCommand() : Command()
{
}

TopicCommand::~TopicCommand()
{
}