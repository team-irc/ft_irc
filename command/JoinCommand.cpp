#include "JoinCommand.hpp"
#include "ft_irc.hpp"

/*
** 처리 안함 리스트
** ERR_BANNEDFROMCHAN
** ERR_BADCHANNELKEY
** ERR_CHANNELISFULL
** ERR_BADCHANMASK
** ERR_NOSUCHCHANNEL
** ERR_TOOMANYCHANNELS
** RPL_TOPIC
*/

void	JoinCommand::run(IrcServer &irc)
{
	int				size;
	std::string		*channel_names;
	std::string		prefix;
	Channel			*channel;
	Socket			*socket = irc.get_current_socket();
	Member			*member;

	if (!deal_exception(irc))
		return ;
	if (socket->get_type() == UNKNOWN)
		return ;
	else if (socket->get_type() == SERVER)
	{
		size = ft::split(_msg.get_param(0), ',', channel_names);
		prefix = _msg.get_prefix();
		std::string		*info;
		ft::split(prefix, '!', info);
		member = irc.get_member(info[0]);

		for (int i = 0; i < size; i++)
		{
			channel = irc.get_channel(channel_names[i]);	
			if (channel == 0)
			{
				channel = new Channel(channel_names[i], member);
				irc.add_channel(channel_names[i], channel);
			}
			else
			{
				channel->add_member(member);
			}
		}
		irc.send_msg_server(socket->get_fd(), _msg.get_msg());
	}
	else if (socket->get_type() == CLIENT)
	{
		size = ft::split(_msg.get_param(0), ',', channel_names);
		member = irc.find_member(socket->get_fd());
		_msg.set_prefix(member->get_nick());
		for (int i = 0; i < size; i++)
		{
			channel = irc.get_channel(channel_names[i]);	
			if (channel == NULL)
			{
				channel = new Channel(channel_names[i], member);
				irc.add_channel(channel_names[i], channel);
			}
			else
			{
				channel->add_member(member);
			}
		}
		irc.send_msg_server(socket->get_fd(), _msg.get_msg());
	}
	std::cout << "Join Command executed.\n";
}

JoinCommand::JoinCommand() : Command()
{
}

JoinCommand::~JoinCommand()
{
}

JoinCommand::JoinCommand(JoinCommand const &copy)
{
	_msg = copy._msg;
}

JoinCommand	&JoinCommand::operator=(JoinCommand const &ref)
{
	_msg = ref._msg;
	return (*this);
}

bool JoinCommand::deal_exception(IrcServer &irc)
{
	int			split_size;
	Channel		*channel;
	std::string	*split_ret;
	Socket		*socket = irc.get_current_socket();

	if (_msg.get_param_size() < 1)
		goto ERR_NEEDMOREPARAMS;
	split_size = ft::split(_msg.get_param(0), ',', split_ret);
	for (int i = 0; i < split_size; ++i)
	{
		channel = irc.get_channel(split_ret[i]);
		if (channel)
		{
			if (channel->find_mode('i'))
				goto ERR_INVITEONLYCHAN;
		}
	}
	delete[] split_ret;
	return (true);

ERR_NEEDMOREPARAMS:
	socket->write(Reply(ERR::NEEDMOREPARAMS(), "JOIN").get_msg().c_str());
	return (false);

ERR_INVITEONLYCHAN:
	delete[] split_ret;
	socket->write(Reply(ERR::INVITEONLYCHAN(), channel->get_name()).get_msg().c_str());
	return (false);
}