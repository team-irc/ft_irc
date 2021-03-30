#include "PartCommand.hpp"
#include "ft_irc.hpp"
#include "Reply.hpp"

void	PartCommand::run(IrcServer &irc)
{
	int				size;
	std::string		*channel_names;
	Channel			*channel;
	Socket			*socket;
	Member			*member;

	if (!deal_exception(irc))
		return ;
	socket = irc.get_current_socket();
	_msg.get_param(0);
	if (socket->get_type() == SERVER)
	{
		size = ft::split(_msg.get_param(0), ',', channel_names);
		member = irc.find_member(socket->get_fd());
		for (int i = 0; i < size; i++)
		{
			channel = irc.get_channel(channel_names[i]);
			// 	channel->delete_member(member);
		}
		irc.send_msg_server(socket->get_fd(), _msg.get_msg()); 
	}
	else if (socket->get_type() == CLIENT) // 클라이언트에서 온 경우
	{
		_msg.set_prefix(member->get_nick());

		size = ft::split(_msg.get_param(0), ',', channel_names);
		member = irc.find_member(socket->get_fd());
		for (int i = 0; i < size; i++)
		{
			channel = irc.get_channel(channel_names[i]);
			// 	channel->delete_member(member);
		}
		irc.send_msg_server(socket->get_fd(), _msg.get_msg());
		// 2. 채널목록으로 채널을 가져온다.
		// 3. 채널의 활성 멤버에서 자기를 제거한다.
		// 4. 채널에 없다면 에러
		// 5. 다른서버에도 메세지를 전송
	}
	else
	{
		return ;
	}
	std::cout << "Part Command executed.\n";
}

PartCommand::PartCommand() : Command()
{
}

PartCommand::~PartCommand()
{
}

PartCommand::PartCommand(PartCommand const &copy)
{
	_msg = copy._msg;
}

PartCommand	&PartCommand::operator=(PartCommand const &ref)
{
	_msg = ref._msg;
	return (*this);
}

bool PartCommand::deal_exception(IrcServer &irc)
{
	int			i = 0;
	int			split_size;
	std::string	*split_ret;
	Channel		*channel;
	Socket		*socket = irc.get_current_socket();

	if (_msg.get_param_size() < 1)
		goto ERR_NEEDMOREPARAMS;
	split_size = ft::split(_msg.get_param(0), ',', split_ret);
	while (i < split_size)
	{
		channel = irc.get_channel(split_ret[i]);
		if (channel == NULL)
			goto ERR_NOSUCHCHANNEL;
		if (!channel->find_member(irc.find_member(socket->get_fd())))
			goto ERR_NOTONCHANNEL;
		++i;
	}
	delete[] split_ret;
	return (true);

ERR_NEEDMOREPARAMS:
	socket->write(Reply(ERR::NEEDMOREPARAMS(), "PART").get_msg().c_str());
	return (false);

ERR_NOSUCHCHANNEL:
	socket->write(Reply(ERR::NOSUCHCHANNEL(), split_ret[i]).get_msg().c_str());
	delete[] split_ret;
	return (false);

ERR_NOTONCHANNEL:
	socket->write(Reply(ERR::NOTONCHANNEL(), split_ret[i]).get_msg().c_str());
	delete[] split_ret;
	return (false);
}