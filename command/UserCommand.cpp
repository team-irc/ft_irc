#include "UserCommand.hpp"
#include "ft_irc.hpp"

// user 메시지 수신
// 1. fd가 있는지 검사(Member 객체 내에 해당 fd를 가지는 값이 있는지)
// 1-1. 있으면 정보 삽입, 다른 서버에 nick을 prefix로 하는 user 메시지 전송
// 1-2. 없으면 fd를 키로 map에 삽입, 다른 서버에 전송은 하지 않음
// param : :prefix USER username hostname servername realname
void	UserCommand::run(IrcServer &irc)
{
	Member	*member = irc.find_member(irc.get_current_socket().get_fd());
	if (member)
	{
		member->set_username(_msg.get_param(0));
		member->set_hostname(_msg.get_param(1));
		member->set_servername(_msg.get_param(2));
		member->set_realname(_msg.get_param(3));
		_msg.set_prefix(member->get_nick());
		irc.send_msg()
	}
	else
	{

	}
	cout << "User command executed.\n";
}

UserCommand::UserCommand() : Command()
{
}

UserCommand::~UserCommand()
{
}

UserCommand::UserCommand(UserCommand const &copy)
{
	_msg = copy._msg;
}

UserCommand	&UserCommand::operator=(UserCommand const &ref)
{
	_msg = ref._msg;
	return (*this);
}