#include "NickCommand.hpp"

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

static std::string get_servername(Socket *socket, IrcServer &irc)
{
	std::map<std::string, Server *>::iterator	begin = irc.get_global_server().begin();
	std::map<std::string, Server *>::iterator	end = irc.get_global_server().end();

	while (begin != end)
	{
		if (begin->second->get_socket() == socket)
			return (begin->first);
		begin++;
	}
	return (NULL);
}

void	NickCommand::run(IrcServer &irc)
{
	Socket		*socket;
	Member		*member;
	std::string	nickname;
	int			hopcount;

	if (_msg.get_param_size() <= 0)
		throw (Reply(ERR::NONICKNAMEGIVEN()));
	socket = irc.get_current_socket();
	if (socket->get_type() == CLIENT) // (닉네임 변경하는 경우)
	{
		nickname = _msg.get_param(0);
		member = irc.get_member(nickname);
		if (!check_nick(nickname))
			throw (Reply(ERR::ERRONEUSNICKNAME(), nickname));
		if (member)
			throw (Reply(ERR::NICKNAMEINUSE(), nickname));
		member = irc.find_member(socket->get_fd());
		_msg.set_prefix(member->get_nick()); // 닉네임 변경 전, 이전 닉네임을 프리픽스에 추가.
		irc.get_user_history().push_back(*member);
		irc.delete_member(member->get_nick());  // 2. global_user에서 삭제
		member->set_nick(nickname); // 3. member 닉네임 변경
		irc.add_member(nickname, member); // 4. global_user에 새로 추가
		irc.send_msg_server(socket->get_fd(), _msg.get_msg()); 
		socket->set_linkname(member->get_nick() + "~!" + member->get_username() + "@" + member->get_hostname());
	}
	else if (socket->get_type() == UNKNOWN) // UNKNOWN에서 온 경우(추가)
	{
		nickname = _msg.get_param(0);
		member = irc.get_member(nickname);
		if (!check_nick(nickname))
			throw (Reply(ERR::ERRONEUSNICKNAME(), nickname));
		if (member)
			throw (Reply(ERR::NICKNAMEINUSE(), nickname));
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
				// RFC 2813
				_msg.set_param_at(1, "1");

				irc.send_msg_server(socket->get_fd(), _msg.get_msg()); // 2. nick 메세지 전송
				// 3. USER 메세지 전송
				std::string		str;
				str = "USER " +member->get_username() + " " + member->get_hostname() + " " + member->get_servername() + " " + member->get_realname();
				Message		user_msg(str.c_str());
				user_msg.set_prefix(nickname);
				irc.send_msg_server(socket->get_fd(), user_msg.get_msg());
				irc.print_motd();
				socket->set_linkname(member->get_nick() + "~!" + member->get_username() + "@" + member->get_hostname());
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
		else // 새로운 NICK 등록 (User 들어오기 전, 멤버에 추가만)
		{
			// 해당 소켓에서 이전에 등록한 nick 이 있는지 체크
			if ((member = irc.find_member(socket->get_fd())) != NULL)
				irc.delete_member(member->get_nick());
			else
			{
				member = new Member();
				member->set_fd(socket->get_fd());
				member->set_socket(irc.get_current_socket());
			}
			member->set_nick(nickname);
			irc.add_member(nickname, member);
		}
	}
	else if (socket->get_type() == SERVER) // 서버에서 온 경우 (추가 or 변경 : prefix 여부로 구분)
	{
		nickname = _msg.get_param(0);
		member = irc.get_member(nickname);
		if (_msg.get_prefix().empty()) // 닉네임 추가하는 경우 [recv] Nick t1 :2
		{ // <nickname> <hopcount> <username> <host> <servertoken> <umode> <realname>
			// 1. 멤버 생성 및 정보 입력
			member = new Member();
			member->set_nick(nickname);
			member->set_fd(socket->get_fd());
			member->set_socket(irc.get_current_socket());
			// 2. 멤버 추가
			irc.add_member(nickname, member);
			// msg는 user에서 전송함
			member->set_hopcount(hopcount);
		}
		else // 닉네임 변경하는 경우
		{
			member = irc.get_member(_msg.get_prefix()); // 1. prefix의 nick을 통해 user_map에서 멤버 찾음
			irc.get_user_history().push_back(*member);
			irc.delete_member(member->get_nick());
			member->set_nick(nickname);
			irc.add_member(nickname, member);	// 2. 해당 멤버 삭제 후 닉네임 변경하여 추가
			irc.send_msg_server(socket->get_fd(), _msg.get_msg());
		}
	}
	std::cout << std::endl;
}

NickCommand::NickCommand() : Command()
{
}

NickCommand::~NickCommand()
{
}

bool NickCommand::check_nick(const std::string &nick)
{
	const char			special[] = {'-', '|', '\\', '`', '^', '{', '}', '\0'};

	if (!ft::isalpha(nick[0]))
		return (false);
	for (int i = 1; i < nick.length(); ++i)
	{
		if (((!ft::strchr(special, nick[i])) && !ft::isalpha(nick[i]) && !ft::isdigit(nick[i])))
			return (false);
	}
	return (true);
}