#include "NickCommand.hpp"
#include "ft_irc.hpp"
#include <cctype>

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

	if (_msg.get_param_size() <= 0)
		return (socket->write((Reply(ERR::NONICKNAMEGIVEN()).get_msg()).c_str()));
	socket = irc.get_current_socket();
	nickname = _msg.get_param(0);
	member = irc.get_member(nickname);
	//if (this->nick_name_check(nickname))
		//return (socket->write((Reply(ERR::ERRONEUSNICKNAME(), nickname).get_msg()).c_str()));
	if (member)
		return (socket->write((Reply(ERR::ALREADYREGISTRED()).get_msg()).c_str()));
	if (socket->get_type() == UNKNOWN) // UNKNOWN에서 온 경우(추가)
	{
		member = irc.get_member(socket->get_fd());
		_msg.set_param_at(1, "0");
		// USER가 먼저 들어온 경우
		if (member)
		{
			member->set_nick(nickname); // 1. 닉네임 설정
			// 2. 패스워드 체크
			if (irc.check_pass(socket))
			{
				// 등록 완료. NICK, USER 순서로 메시지 전송
				socket->set_type(CLIENT); // 1. 소켓타입 변경
				irc.send_msg_server(socket->get_fd(), _msg.get_msg()); // 2. nick 메세지 전송
				// 3. USER 메세지 전송
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
				socket->write("Bad password\n");
		}
		else // 새로운 NICK 등록
		{
			member = new Member();
			member->set_nick(nickname);
			member->set_fd(socket->get_fd());
			_msg.set_prefix(nickname);
			irc.add_member(nickname, member);
		}
	}
	else if (socket->get_type() == CLIENT) // 클라이언트에서 NICK 메세지가 온 경우(변경)
	{
		// nick 변경
		member = irc.get_member(socket->get_fd());
		_msg.set_prefix(member->get_nick()); // 닉네임 변경 전, 이전 닉네임을 프리픽스에 추가.
		irc.delete_member(member->get_nick());  // 2. global_user에서 삭제
		member->set_nick(nickname); // 3. member 닉네임 변경
		irc.add_member(nickname, member); // 4. global_user에 새로 추가 

		_msg.set_param_at(1, "0"); // 2. 다른서버로 전송
		irc.send_msg_server(socket->get_fd(), _msg.get_msg());
	}
	else if (socket->get_type() == SERVER) // 서버에서 온 경우 (추가 or 변경 : prefix 여부로 구분)
	{
		hopcount = ft::atoi(_msg.get_param(1).c_str());
		hopcount++;
		_msg.set_param_at(1, std::to_string(hopcount));
		if (_msg.get_prefix().empty())
		{
			// NICK 추가
			member = new Member();
			member->set_nick(nickname);
			member->set_fd(socket->get_fd());
			irc.add_member(nickname, member);
		}
		else
		{
			// 해당 nick 변경
			member = irc.get_member(_msg.get_prefix()); // 1. prefix의 nick을 통해 user_map에서 멤버 찾음
			irc.delete_member(member->get_nick());
			member->set_nick(nickname);
			irc.add_member(nickname, member);	// 2. 해당 멤버 삭제 후 닉네임 변경하여 추가
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

/*bool nick_name_check(std::string & nick)
{
	const char special[] = {'-', '|', '\\', '`', '^', '{', '}'};

	if (std::isalpha(nick[0]))
		return (false);
	for (int i = 1; i < nick.length(); ++i)
	{
	}
	return (false);
}*/