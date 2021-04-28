#include "KillCommand.hpp"

/*
      Command: KILL
   Parameters: <nickname> <comment>

   The KILL message is used to cause a client-server connection to be
   closed by the server which has the actual connection.  KILL is used
   by servers when they encounter a duplicate entry in the list of valid
   nicknames and is used to remove both entries.  It is also available
   to operators.

   Clients which have automatic reconnect algorithms effectively make
   this command useless since the disconnection is only brief.  It does
   however break the flow of data and can be used to stop large amounts
   of being abused, any user may elect to receive KILL messages
   generated for others to keep an 'eye' on would be trouble spots.

   In an arena where nicknames are required to be globally unique at all
   times, KILL messages are sent whenever 'duplicates' are detected
   (that is an attempt to register two users with the same nickname) in
   the hope that both of them will disappear and only 1 reappear.

   The comment given must reflect the actual reason for the KILL.  For
   server-generated KILLs it usually is made up of details concerning
   the origins of the two conflicting nicknames.  For users it is left
   up to them to provide an adequate reason to satisfy others who see
   it.  To prevent/discourage fake KILLs from being generated to hide
   the identify of the KILLer, the comment also shows a 'kill-path'
   which is updated by each server it passes through, each prepending
   its name to the path.

   Numeric Replies:

           ERR_NOPRIVILEGES                ERR_NEEDMOREPARAMS
           ERR_NOSUCHNICK                  ERR_CANTKILLSERVER


   KILL David (csd.bu.edu <- tolsun.oulu.fi)
                                   ; Nickname collision between csd.bu.edu
                                   and tolson.oulu.fi


   NOTE:
   It is recommended that only Operators be allowed to kill other users
   with KILL message.  In an ideal world not even operators would need
   to do this and it would be left to servers to deal with.

   KILL 메시지는 실제 연결이있는 서버에 의해 클라이언트-서버 연결이 닫히도록하는 데 사용됩니다.
   KILL은 유효한 별명 목록에서 중복 항목을 발견 할 때 서버에서 사용되며 두 항목을 모두 제거하는 데 사용됩니다. 
   운영자도 사용할 수 있습니다.

   자동 재 연결 알고리즘이있는 클라이언트는 연결 해제가 짧기 때문에이 명령을 효과적으로 쓸모 없게 만듭니다. 
   그러나 그것은 데이터의 흐름을 깨뜨리고 많은 양의 남용을 막는 데 사용될 수 있습니다. 
   어떤 사용자 든 다른 사람들이 문제가되는 '시선'을 유지하기 위해 생성 된 KILL 메시지를 수신하도록 선택할 수 있습니다.

   닉네임이 항상 전역 적으로 고유해야하는 경기장에서 '중복'이 감지 될 때마다 (즉, 동일한 닉네임으로 두 명의 사용자를 등록하려는 시도)
   KILL 메시지가 전송되어 둘 다 사라지고 1이 다시 나타납니다.
   주어진 설명은 KILL의 실제 이유를 반영해야합니다. 서버 생성 KILL의 경우 일반적으로 충돌하는 두 닉네임의 출처에 관한 세부 정보로 구성됩니다. 
   사용자에게 그것을 보는 다른 사람들을 만족시킬 적절한 이유를 제공하는 것은 그들에게 달려 있습니다. 
   KILLer의 신원을 숨기기 위해 가짜 KILL이 생성되는 것을 방지 / 방지하기 위해, 주석은 통과하는 각 서버에 의해 업데이트되는 'kill-path'도 표시하며, 각 서버는 경로에 이름을 추가합니다.

   참고 : 운영자 만 KILL 메시지로 다른 사용자를 죽일 수 있도록 허용하는 것이 좋습니다. 이상적인 세계에서는 운영자조차도이를 수행 할 필요가 없으며 처리 할 서버에 맡겨 질 것입니다.
*/

// static void		kill(IrcServer &irc, Socket *socket)
// {

// }

static void		quit_from_joined_channel(IrcServer &irc, Member	*member)
{
	std::set<Channel *>::iterator	channel_iter;
	Channel		*channel;

	channel_iter = member->get_joined_channels().begin();
	while (channel_iter != member->get_joined_channels().end())
	{
		channel = (*channel_iter);
		channel->delete_member(member); // 채널의 멤버 목록에서 제거
		if (channel->get_members().empty()) 
		{
			irc.delete_channel(channel->get_name()); // _global_channel에서 제거
			delete channel; // 채널 인스턴스 제거
		}
		++channel_iter;
	}
}

static void		kill_member(IrcServer &irc, Member *target, Member *oper, Message message)
{
	std::string		msg;
	if (target->get_socket()->get_type() == CLIENT) // 3. 직접 연결되어 있다면 소켓까지 제거
	{
		if (irc.get_current_socket()->get_type() == CLIENT)
		{
			target->get_socket()->write(std::string("ERROR :KILLed by " + oper->get_nick() + " " + message.get_param(1) + "\n").c_str());
			msg = ":" + target->get_nick() + " QUIT " + message.get_param(1) + " :" + irc.get_serverinfo().SERVER_NAME + "\n"; 
		}
		else if (irc.get_current_socket()->get_type() == SERVER)
		{
			target->get_socket()->write(
					std::string("ERROR :KILLed by " + message.get_prefix() + message.get_param(1) + " -> " + irc.get_serverinfo().SERVER_NAME + "\n")
					.c_str());
			msg = ":" + target->get_nick() + " QUIT " + message.get_param(1) + " -> " + irc.get_serverinfo().SERVER_NAME + "\n";			
		}
		irc.delete_member(target->get_nick());
		quit_from_joined_channel(irc, target); // 1. 접속중인 채널에서 제거
		irc.get_user_history().push_back(*target); // 2. whowas를 위해 멤버 정보 저장
		irc.get_socket_set().remove_socket(target->get_socket());
		delete target->get_socket();
		delete target; // 4. 멤버 제거
		irc.send_msg_server(3, msg.c_str()); // QUIT 전송
	}
	else
	{
		if (irc.get_current_socket()->get_type() == CLIENT)
			msg = " :" + irc.get_serverinfo().SERVER_NAME;
		else if (irc.get_current_socket()->get_type() == SERVER)
			msg = " -> " + irc.get_serverinfo().SERVER_NAME;
		if (message.get_prefix().empty())
			message.set_prefix(oper->get_nick());
		message.set_param_at(1, message.get_param(1) + msg);
		target->get_socket()->write(message.get_msg());
	}
}

void	KillCommand::run(IrcServer &irc)
{
	Socket			*socket;
	Member			*oper;
	Member			*target;

	socket = irc.get_current_socket();
	if (socket->get_type() == CLIENT)
	{
		if (_msg.get_param_size() != 1 && _msg.get_param_size() != 2)
			throw (Reply(ERR::NEEDMOREPARAMS(), _msg.get_command()));
		oper = irc.find_member(socket->get_fd());
		if (oper->check_mode('o', true))
			throw (Reply(ERR::NOPRIVILEGES()));
		target = irc.get_member(_msg.get_param(0));
		if (target == NULL)
			throw (Reply(ERR::NOSUCHNICK(), _msg.get_param(0)));
		_msg.set_prefix(irc.get_serverinfo().SERVER_NAME);
		kill_member(irc, target, oper, _msg);
	}
	else if (socket->get_type() == SERVER)
	{
		target = irc.get_member(_msg.get_param(0));
		kill_member(irc, target, NULL, _msg);
	}
	else if (socket->get_type() == UNKNOWN)
		throw(Reply(ERR::NOTREGISTERED()));
}

KillCommand::KillCommand() : Command()
{
}

KillCommand::~KillCommand()
{
}