#include "TopicCommand.hpp"
#include "ft_irc.hpp"

void	TopicCommand::run(IrcServer &irc)
{
	Socket			*socket;
	Channel			*channel;
	Member			*member;
	std::string		channel_name;
	std::string		topic;

	socket = irc.get_current_socket();
	if (socket->get_type() != SERVER && socket->get_type() != CLIENT)
		return ;
	// 공통 확인 사항(1. 채널에 속한 멤버인지? 2. 채널이 존재하는지? 3. 채널에 토픽이 존재하는지? 4. 채널 모드가 허용하는지?)
	if (socket->get_type() == SERVER)
		member = irc.get_member(_msg.get_prefix());
	member = irc.find_member(socket->get_fd());

	channel_name = _msg.get_param(0);
	channel = irc.get_channel(channel_name);
	if (!channel)
		throw(Reply(ERR::NOSUCHCHANNEL(), channel_name));
	if (!channel->find_member(member))
		throw(Reply(ERR::NOTONCHANNEL(), channel_name));
	if (_msg.get_param_size() == 1) // 1. 토픽 조회 
	{
		topic = channel->get_topic();
		if (topic.empty())
			throw(Reply(RPL::NOTOPIC(), channel_name));
		else
		{
			socket->write(Reply(RPL::TOPIC(), channel_name, topic).get_msg().c_str());
		}
	}
	else // 2. 토픽 설정 (채널모드 i 라면 operator만 가능)
	{
		int	flag;
		if (channel->check_mode('i', flag) == 1 && !channel->is_operator(member))
			throw(Reply(ERR::CHANOPRIVSNEEDED(), channel_name));
		else
		{
			topic = _msg.get_param(1);
			channel->set_topic(topic);
			if (_msg.get_prefix().empty())
				_msg.set_prefix(member->get_nick());
			irc.send_msg_server(socket->get_fd(), _msg.get_msg());
		}
	}
}

TopicCommand::TopicCommand() : Command()
{
}

TopicCommand::~TopicCommand()
{
}

TopicCommand::TopicCommand(TopicCommand const &copy)
{
	_msg = copy._msg;
}

TopicCommand	&TopicCommand::operator=(TopicCommand const &ref)
{
	_msg = ref._msg;
	return (*this);
}