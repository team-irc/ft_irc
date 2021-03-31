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

	if (_msg.get_param_size() < 1)
		throw (Reply(ERR::NEEDMOREPARAMS(), "JOIN"));
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
				member->add_channel(channel);
			}
			else
			{
				member->add_channel(channel);
				channel->add_member(member);
			}
		}
		irc.send_msg_server(socket->get_fd(), _msg.get_msg());
		std::string msg = ":" + irc.get_servername() + " " + std::to_string(RPL::TOPIC().ERRNO) + " " + member->get_nick() + " ";
		socket->write(msg.c_str());
		socket->write(Reply(RPL::TOPIC(), channel->get_name(), channel->get_topic()).get_msg().c_str());
		
		// :u2!~u2a@121.135.181.42 JOIN :#abc
		// :irc.example.net 332 u2 #abc :hello world // RPL_TOPIC
		// :irc.example.net 333 u2 #abc u1 1617164706 // 
		// :irc.example.net 353 u2 = #abc :u2 @u1 // RPL_NAMREPLY
		// :irc.example.net 366 u2 #abc :End of NAMES list // RPL_ENDOFNAMES
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
				member->add_channel(channel);
			}
			else
			{
				member->add_channel(channel);
				channel->add_member(member);
			}
		}
		irc.send_msg_server(socket->get_fd(), _msg.get_msg());
	}
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