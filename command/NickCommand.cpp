#include "NickCommand.hpp"
#include "ft_irc.hpp"

/*
** 	ERR_NICKCOLLISION 처리 안함
*/

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

/*
	Command: NICK
	Parameters: <nickname> [ <hopcount> ]

	NICK 메시지는 사용자에게 별명을 부여하거나 이전 별명을 변경하는 데 사용됩니다. 
	<hopcount> 매개 변수는 닉이 홈 서버에서 얼마나 멀리 떨어져 있는지를 나타내는 서버에서만 사용됩니다. 
	로컬 연결의 홉 수는 0입니다. 클라이언트가 제공하는 경우 무시해야합니다.

	다른 클라이언트의 동일한 별칭을 이미 알고있는 서버에 NICK 메시지가 도착하면 별칭 충돌이 발생합니다.
	별칭 충돌의 결과로 별칭의 모든 인스턴스가 서버의 데이터베이스에서 제거되고 다른 모든 서버의 데이터베이스에서 별칭을 제거하기 위해 KILL 명령이 실행됩니다. 
	충돌을 일으키는 NICK 메시지가 별명 변경 인 경우 원래 (이전) 별명도 제거해야합니다.

	서버가 직접 연결된 클라이언트로부터 동일한 NICK를 받으면 로컬 클라이언트에 ERR_NICKCOLLISION을 발행하고 NICK 명령을 삭제하고 킬을 생성하지 않을 수 있습니다.

	NICK message is used to give user a nickname or change the previous
	one.  The <hopcount> parameter is only used by servers to indicate
	how far away a nick is from its home server.  A local connection has
	a hopcount of 0.  If supplied by a client, it must be ignored.

	If a NICK message arrives at a server which already knows about an
	identical nickname for another client, a nickname collision occurs.
	As a result of a nickname collision, all instances of the nickname
	are removed from the server's database, and a KILL command is issued
	to remove the nickname from all other server's database. If the NICK
	message causing the collision was a nickname change, then the
	original (old) nick must be removed as well.

	If the server recieves an identical NICK from a client which is
	directly connected, it may issue an ERR_NICKCOLLISION to the local
	client, drop the NICK command, and not generate any kills.

	Numeric Replies:

			ERR_NONICKNAMEGIVEN             ERR_ERRONEUSNICKNAME
			ERR_NICKNAMEINUSE               ERR_NICKCOLLISION

	Example:

	NICK Wiz                        ; Introducing new nick "Wiz".

	:WiZ NICK Kilroy                ; WiZ changed his nickname to Kilroy.
*/

void	NickCommand::run(IrcServer &irc)
{
	Socket		*socket;
	Member		*member;
	std::string	nickname;
	int			hopcount;

	if (_msg.get_param_size() <= 0)
		throw (Reply(ERR::NONICKNAMEGIVEN()));
	socket = irc.get_current_socket();
	nickname = _msg.get_param(0);
	member = irc.get_member(nickname);
	if (!check_nick(nickname))
		throw (Reply(ERR::ERRONEUSNICKNAME(), nickname));
	if (member)
		throw (Reply(ERR::NICKNAMEINUSE(), nickname));
	if (socket->get_type() == UNKNOWN) // UNKNOWN에서 온 경우(추가)
	{
		member = irc.get_member(socket->get_fd());
		_msg.set_param_at(1, "1");
		// USER가 먼저 들어온 경우
		if (member)
		{
			// 해당 시점에서의 global_map에는 nick이 아닌 fd값이 key로 설정되어 있으며 nick으로 새롭게 추가하거나 완전히 제거해야 함
			irc.delete_member(member->get_nick());
			member->set_nick(nickname); // 1. 닉네임 설정
			// 2. 패스워드 체크
			if (irc.check_pass(socket))
			{
				// 등록 완료. NICK, USER 순서로 메시지 전송
				// global_map에 nick으로 새롭게 추가함
				irc.add_member(nickname, member);
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
			{
				// 해당 클라이언트 제거 필요, 해당 시점에서 global user에는 있지만 메시지 전파가 되지 않았으므로 global user에서만 제거
				socket->write("Bad password\n");
				delete member;
				irc.get_socket_set().remove_socket(irc.get_current_socket());
				delete irc.get_current_socket();
			}
		}
		else // 새로운 NICK 등록
		{
			member = new Member();
			member->set_nick(nickname);
			member->set_fd(socket->get_fd());
			member->set_socket(irc.get_current_socket());
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

		_msg.set_param_at(1, "1"); // 2. 다른서버로 전송
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
			member->set_socket(irc.get_current_socket());
			irc.add_member(nickname, member);
			irc.send_msg_server(socket->get_fd(), _msg.get_msg());
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

bool NickCommand::check_nick(const std::string &nick)
{
	const char			special[] = {'-', '|', '\\', '`', '^', '{', '}'};

	if (!ft::isalpha(nick[0]))
		return (false);
	for (int i = 1; i < nick.length(); ++i)
	{
		if (((!ft::strchr(special, nick[i])) && !ft::isalpha(nick[i]) && !ft::isdigit(nick[i])))
			return (false);
	}
	return (true);
}