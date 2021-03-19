#include "JoinCommand.hpp"
#include "ft_irc.hpp"

void	JoinCommand::run(IrcServer &irc)
{
	int				size;
	std::string		*channel_names;
	Channel			*channel;
	Socket			*socket;
	Member			*member;

	socket = irc.get_current_socket();
	size = ft::split(_msg.get_param(0), ',', channel_names);
	member = irc.find_member(socket->get_fd());
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