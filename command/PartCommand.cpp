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
	if (socket->get_type() == "SERVER")
	{
		// 1. 
	}
	else if (socket->get_type() == "CLIENT") // 클라이언트에서 온 경우
	{
		// 1. 나가려는 채널 목록을 받아온다. (fd이용)
		// 2. 채널목록으로 채널을 가져온다.
		// 3. 채널의 활성 멤버에서 자기를 제거한다.
		// 4. 채널에 없다면 에러
		// 5. 다른서버에도 메세지를 전송
	}
	else
	{
		
	}
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