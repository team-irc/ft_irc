#include "NickCommand.hpp"
#include "ft_irc.hpp"

// nick 메시지 수신
// 1. nick이 global_user에 있는지 검사
// - 있으면 이미 등록된 상태 반환
// 2.없는 경우
// 2-1. fd를 검사(키 값으로 fd가 있는지)
// 2-1-1. 있으면 해당 Member의 nick 변경 후 추가(기본 값 제거), nick 메시지, user메시지 다른 서버로 전송
// 2-1-2. 없으면 새로운 Member 생성, nick을 키로 map에 삽입, nick 메시지 전송

// user 메시지 수신
// 1. fd가 있는지 검사(Member 객체 내에 해당 fd를 가지는 값이 있는지)
// 1-1. 있으면 정보 삽입, 다른 서버에 nick을 prefix로 하는 user 메시지 전송
// 1-2. 없으면 fd를 키로 map에 삽입, 다른 서버에 전송은 하지 않음

void	NickCommand::run(IrcServer &irc)
{
	Socket		*socket;
	Member		*member;
	std::string	nickname;
	int			hopcount;

	socket = irc.get_current_socket();
	nickname = _msg.get_param(0);
	member = irc.get_member(nickname);
	if (member)
	{
		socket->write("ERR_ALREADYREGISTERED");
	}
	else
	{
		member = irc.get_member(socket->get_fd());
		if (socket->get_type() == CLIENT || socket->get_type() == UNKNOWN) // 메세지를 받은 소켓의 타입이 CLIENT에서 홉카운트는 0으로 설정 
			_msg.set_param_at(1, "0");
		else // SERVER에서 
		{
			hopcount = ft::atoi(_msg.get_param(1).c_str());
			hopcount++;
			_msg.set_param_at(1, std::to_string(hopcount));
		}
		if (member)
		{
			// NICK 메세지 전송
			member->set_nick(nickname);
			_msg.get_param(1);
			_msg.set_param_at(1, "");

			irc.send_msg_server(socket->get_fd(), _msg.get_msg());

			// USER 메세지 전송
			std::string		str;
			str += "USER ";
			str += member->get_username();
			str += " ";
			str += member->get_hostname();
			str += " ";
			str += member->get_servername();
			str += " ";
			str += member->get_realname();
		
			Message		user_msg(str.c_str());
			user_msg.set_prefix(nickname);
			irc.send_msg_server(socket->get_fd(), user_msg.get_msg());
		}
		else
		{
			// 글로벌 DB에 추가 후 NICK 메세지 전송
			member = new Member();
			member->set_nick(nickname);
			member->set_fd(socket->get_fd());
			irc.add_member(nickname, member);
			irc.send_msg_server(socket->get_fd(), _msg.get_msg());
		}
	}
	std::cout << "Nick Command executed.\n";
}

NickCommand::NickCommand() : Command()
{
}

NickCommand::~NickCommand()
{
}

NickCommand::NickCommand(NickCommand const &copy)
{
	_msg = copy._msg;
}

NickCommand	&NickCommand::operator=(NickCommand const &ref)
{
	_msg = ref._msg;
	return (*this);
}