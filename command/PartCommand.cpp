#include "PartCommand.hpp"
#include "ft_irc.hpp"

void	PartCommand::run(IrcServer &irc)
{
	int				size;
	std::string		*channel_names;
	Channel			*channel;
	Socket			*socket;
	Member			*member;

	socket = irc.get_current_socket();
	size = ft::split(_msg.get_param(0), ',', channel_names);
	member = irc.find_member(socket->get_fd());
	for (int i = 0, i < size, i++)
	{
		channel = irc.get_channel(channel_names[i]);	
		if (channel == 0)
		{
			throw(Error("ERR_NO SUCH CHANNEL"));
		}
		else
		{
			if (channel->find_member(member))
				channel->delete_member(member);
			else
				throw(Error("ERR_NOT ON CHANNEL"));
		}
		irc.send_msg_server(socket.get_fd(), _msg.get_msg());
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