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
	{
		return ;
	}
	// 공통 확인 사항(1. 채널에 속한 멤버인지? 2. 채널이 존재하는지? 3. 채널에 토픽이 존재하는지? 4. 채널 모드가 허용하는지?)
	if (socket->get_type() == SERVER)
		member = irc.get_member(_msg.get_prefix());
	member = irc.find_member(socket->get_fd());
	channel_name = _msg.get_param(0);
	channel = irc.get_channel(channel_name);
	if (!channel)
		socket->write("ERR NO SUCH CHANNEL");
	if (!channel->find_member(member))
		socket->write("ERR NOT ON CHANNEL");
	if (_msg.get_param_size() == 1) // 토픽 조회 
	{
		topic = channel->get_topic();
		if (topic.empty())
			socket->write("RPL NO TOPIC");
		else
			socket->write(topic.c_str()); // RPL TOPIC
	}
	else // 토픽 설정 (1. 채널 모드가 )
	{
		topic = _msg.get_param(1);
		channel->set_topic(topic);
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