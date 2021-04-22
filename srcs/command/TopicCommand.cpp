#include "TopicCommand.hpp"
#include "ft_irc.hpp"

/*

*/

void	TopicCommand::run(IrcServer &irc)
{
	Socket			*socket;
	Channel			*channel;
	Member			*member;
	std::string		channel_name;
	std::string		topic;

	socket = irc.get_current_socket();
	if (socket->get_type() != SERVER && socket->get_type() != CLIENT) // 서버나  클라이언트에서 보낸 메세지가 아니면 무시
		return ;
	if (socket->get_type() == SERVER) // 서버에서 왔다면, prefix를 통해서 멤버를 찾는다.
		member = irc.get_member(_msg.get_prefix());
	member = irc.find_member(socket->get_fd()); // 클라이언트에서 왔다면, fd를 통해서 멤버를 찾는다.
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
		{
			socket->write(Reply(RPL::TOPIC(), channel_name, topic).get_msg().c_str());
		}
	}
	else // 2. 토픽 설정하는 경우
	{
		if (channel->check_mode('t', 0) == 1 && !channel->is_operator(member)) // 채널 모드가 i라면 오퍼레이터만 가능
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