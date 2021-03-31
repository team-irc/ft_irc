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

	if (_msg.get_param_size() < 1)
		throw (Reply(ERR::NEEDMOREPARAMS(), "PART"));
	socket = irc.get_current_socket();
	_msg.get_param(0);
	if (socket->get_type() == SERVER)
	{
		size = ft::split(_msg.get_param(0), ',', channel_names);
		std::string	nickname = _msg.get_prefix();
		member = irc.get_member(nickname);
		for (int i = 0; i < size; i++)
		{
			channel = irc.get_channel(channel_names[i]);
			if (channel == NULL)
				throw (Reply(ERR::NOSUCHCHANNEL(), channel_names[i]));
			if (!channel->find_member(member))
				throw (Reply(ERR::NOTONCHANNEL(), channel_names[i]));
			channel->delete_member(member);
			member->delete_channel(channel);
		}
		irc.send_msg_server(socket->get_fd(), _msg.get_msg()); 
	}
	else if (socket->get_type() == CLIENT) // 클라이언트에서 온 경우
	{
		size = ft::split(_msg.get_param(0), ',', channel_names);
		member = irc.find_member(socket->get_fd());
		_msg.set_prefix(member->get_nick());
		for (int i = 0; i < size; i++)
		{
			channel = irc.get_channel(channel_names[i]);
			if (channel == NULL)
				throw (Reply(ERR::NOSUCHCHANNEL(), channel_names[i]));
			if (!channel->find_member(member))
				throw (Reply(ERR::NOTONCHANNEL(), channel_names[i]));
			channel->delete_member(member);
			member->delete_channel(channel);
		}
		irc.send_msg_server(socket->get_fd(), _msg.get_msg());
	}
	else
	{
		return ;
	}
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