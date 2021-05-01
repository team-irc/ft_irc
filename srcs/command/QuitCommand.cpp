#include "QuitCommand.hpp"

/*
      Command: QUIT
   Parameters: [<Quit message>]

   A client session is ended with a quit message.  The server must close
   the connection to a client which sends a QUIT message. If a "Quit
   Message" is given, this will be sent instead of the default message,
   the nickname.

   When netsplits (disconnecting of two servers) occur, the quit message
   is composed of the names of two servers involved, separated by a
   space.  The first name is that of the server which is still connected
   and the second name is that of the server that has become
   disconnected.

   If, for some other reason, a client connection is closed without  the
   client  issuing  a  QUIT  command  (e.g.  client  dies and EOF occurs
   on socket), the server is required to fill in the quit  message  with
   some sort  of  message  reflecting the nature of the event which
   caused it to happen.

   Numeric Replies:

           None.

   Examples:

   QUIT :Gone to have lunch        ; Preferred message format.
*/

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

static void		quit(IrcServer &irc, Member *member)
{
	if (member)
	{
		irc.delete_member(member->get_nick()); // 1. 멤버를 _global_user에서 지운다.
		quit_from_joined_channel(irc, member); // 2. 접속중인 채널에서 제거
		irc.get_user_history().push_back(*member); // 3. whowas를 위해 멤버 정보 저장
		if (member->get_socket()->get_type() == CLIENT)
		{
			irc.get_socket_set().remove_socket(member->get_socket()); // 4. 소켓제거
			delete member->get_socket();
		}
		delete member; // 5. 멤버 제거
	}
}

static void		notice_to_channel_member(Member * member, std::string quit_message)
{
	std::set<Channel *>::iterator	first;
	std::set<Channel *>::iterator	last;
	std::string						msg;

	first = member->get_joined_channels().begin();
	last = member->get_joined_channels().end();
	if (quit_message[0] == ':')
		quit_message.erase(0, 1);
	while (first != last)
	{
		msg = ':' + member->get_nick() + "!~" + member->get_username() + "@" + member->get_hostname() + " QUIT :";
		msg += quit_message + '\n';
		(*first)->send_msg_to_members(msg.c_str());
		msg.clear();
		++first;
	}
}

void	QuitCommand::run(IrcServer &irc)
{
	Socket		*socket;
	Member		*member;

	socket = irc.get_current_socket();
	if (socket->get_type() == CLIENT)
	{
		member = irc.find_member(socket->get_fd()); // 1. 멤버를 찾는다.
		if (!member->get_joined_channels().empty())
			notice_to_channel_member(member, _msg.get_param_size() >= 1 ? _msg.get_param(0) : member->get_nick());
		if (member)
			_msg.set_prefix(member->get_nick()); 	// 2. 메세지를 전파하기 위해 닉네임을 프리픽스로 설정
		quit(irc, member);							// 3. 제거
		irc.send_msg_server(0, _msg.get_msg()); 	// 4. 메세지 전파
	}
	else if (socket->get_type() == SERVER)
	{
		member = irc.get_member(_msg.get_prefix());				// 1. prefix로 멤버를 찾는다.
		quit(irc, member);										// 2. 멤버 제거
		irc.send_msg_server(socket->get_fd(), _msg.get_msg());	// 3. 다른서버에도 메세지를 보낸다.
	}
	else if (socket->get_type() == UNKNOWN)
	{
		member = irc.find_member(socket->get_fd());	// 1. fd값으로 멤버를 찾는다.
		if (member)
		{
			irc.delete_member(member->get_nick());	// 2. 멤버를 _global_user에서 지운다.
			quit_from_joined_channel(irc, member);
			delete member; 							//멤버 인스턴스를 제거
		}
		irc.get_socket_set().remove_socket(socket);	// socket_set에서 제거
		delete socket; 								// 3. 소켓 연결을 끊는다. 다른 서버에는 전송하지 않는다. 다른서버에는 이 멤버가 등록되지 않은 상태이므로
	}
}

QuitCommand::QuitCommand() : Command()
{
}

QuitCommand::~QuitCommand()
{
}