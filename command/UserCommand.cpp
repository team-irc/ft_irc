#include "UserCommand.hpp"
#include "ft_irc.hpp"

// user 메시지 수신
// 0. prefix가 있다면 다른 서버에서 전송된 상태
// 1. fd가 있는지 검사(Member 객체 내에 해당 fd를 가지는 값이 있는지)
// 1-1. 있으면 정보 삽입, 다른 서버에 nick을 prefix로 하는 user 메시지 전송
// 1-2. 없으면 fd를 키로 map에 삽입, 다른 서버에 전송은 하지 않음
// param : :prefix USER username hostname servername realname
void	UserCommand::run(IrcServer &irc)
{
	std::string	prefix = _msg.get_prefix();
	Member	*member;

	if (prefix.empty())
	{
		// 자신에게 연결된 클라이언트에서 전송된 경우
		member = irc.find_member(irc.get_current_socket()->get_fd());
		if (member)
			insert_info(member, irc);
		else
		{
			// fd가 키 값으로 map에 삽입됨, 전송은 하지 않음(nick이 없으므로)
			std::string	key = std::to_string(irc.get_current_socket()->get_fd());
			member = new Member(key, _msg.get_param(0), _msg.get_param(1), _msg.get_param(2), _msg.get_param(3), 0);
			member->set_fd(irc.get_current_socket()->get_fd());
			irc.add_member(key, member);
		}
	}
	else
	{
		// 다른 서버에서 전송된 경우, NICk이 우선적으로 전송되기 때문에 map에 있는 상태
		member = irc.get_member(prefix);
		if (member == NULL)
		{
			member = new Member(prefix, _msg.get_param(0), _msg.get_param(1), _msg.get_param(2), _msg.get_param(3), 0);
			member->set_fd(irc.get_current_socket()->get_fd());
			irc.add_member(prefix, member);
		}
		insert_info(member, irc);
	}
	std::cout << "User command executed.\n";
}

void	UserCommand::insert_info(Member *member, IrcServer &irc)
{
	member->set_username(_msg.get_param(0));
	member->set_hostname(_msg.get_param(1));
	member->set_servername(_msg.get_param(2));
	member->set_realname(_msg.get_param(3));
	_msg.set_prefix(member->get_nick());
	irc.send_msg_server(irc.get_current_socket()->get_fd(), _msg.get_msg());
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