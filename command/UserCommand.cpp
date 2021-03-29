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
	Socket	*sock = irc.get_current_socket();

	if (sock->get_type() == UNKNOWN)
	{
		member = irc.find_member(irc.get_current_socket()->get_fd());
		if (member) // NICK이 먼저 들어온 경우
		{
			insert_info(member, irc);	
		}
		else
		{
			// fd가 키 값으로 map에 삽입됨, 전송은 하지 않음(nick이 없으므로), 전송은 NICK에서
			std::string	key = std::to_string(irc.get_current_socket()->get_fd());
			member = new Member(key, _msg.get_param(0), _msg.get_param(1), _msg.get_param(2), _msg.get_param(3), 0);
			member->set_fd(irc.get_current_socket()->get_fd());
			if (!irc.add_member(key, member)) {
				// KILL 메시지 전송
			}
		}
	}
	else if (sock->get_type() == CLIENT)
	{
		irc.get_current_socket()->write(":servername 462 *(or nick) :Connection already registered\n");
	}
	else if (sock->get_type() == SERVER)
	{
		//SERVER에서 들어오는 경우, NICK->USER 순서이므로 이 단계에서 이미 NICK이 등록 됨
		member = irc.get_member(prefix);
		if (member)
		{
			// nick 내부에서 ADD_MEMBER에 대한 처리를 했으므로 정보 업데이트만 하면 됨
			insert_info(member, irc);
		}
	}
}

void	UserCommand::insert_info(Member *member, IrcServer &irc)
{
	if (irc.get_current_socket()->get_type() == UNKNOWN && irc.check_pass(irc.get_current_socket()))
	{
		// user 메시지 전송
		member->set_username(_msg.get_param(0));
		member->set_hostname(_msg.get_param(1));
		member->set_servername(_msg.get_param(2));
		member->set_realname(_msg.get_param(3));
		// NICK 메시지 전송
		std::string msg = "NICK " + member->get_nick() + " 0\n";
		irc.send_msg_server(irc.get_current_socket()->get_fd(), msg.c_str());

		_msg.set_prefix(member->get_nick());
		irc.send_msg_server(irc.get_current_socket()->get_fd(), _msg.get_msg());
		irc.get_socket_set().change_socket_type(irc.get_current_socket()->get_fd(), CLIENT);

		if (!irc.get_current_socket()->get_pass().empty())
		{
			if (irc.check_pass(irc.get_current_socket()))
			{
				
			}
		}
	}
	else if (irc.get_current_socket()->get_type() == SERVER)
	{
		member->set_username(_msg.get_param(0));
		member->set_hostname(_msg.get_param(1));
		member->set_servername(_msg.get_param(2));
		member->set_realname(_msg.get_param(3));
		std::string msg = "NICK " + member->get_nick() + " 0\n";
		irc.send_msg_server(irc.get_current_socket()->get_fd(), msg.c_str());
		_msg.set_prefix(member->get_nick());
		irc.send_msg_server(irc.get_current_socket()->get_fd(), _msg.get_msg());
	}
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