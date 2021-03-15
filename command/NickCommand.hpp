#ifndef NICK_COMMAND_HPP
# define NICK_COMMAND_HPP

# include "Command.hpp"

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

class NickCommand : public Command
{
private:

public:
	NickCommand();
	virtual ~NickCommand();
	NickCommand(NickCommand const &copy);
	NickCommand	&operator=(NickCommand const &ref);

	void	run(IrcServer &irc);
};

#endif