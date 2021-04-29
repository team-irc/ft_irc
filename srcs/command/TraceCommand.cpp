#include "TraceCommand.hpp"

/*
   Command: TRACE
   Parameters: [<server>]

   TRACE command is used to find the route to specific server.  Each
   server that processes this message must tell the sender about it by
   sending a reply indicating it is a pass-through link, forming a chain
   of replies similar to that gained from using "traceroute".  After
   sending this reply back, it must then send the TRACE message to the
   next server until given server is reached.  If the <server> parameter
   is omitted, it is recommended that TRACE command send a message to
   the sender telling which servers the current server has direct
   connection to.

   If the destination given by "<server>" is an actual server, then the
   destination server is required to report all servers and users which
   are connected to it, although only operators are permitted to see
   users present.  If the destination given by <server> is a nickname,
   they only a reply for that nickname is given.

   Numeric Replies:

           ERR_NOSUCHSERVER

   If the TRACE message is destined for another server, all intermediate
   servers must return a RPL_TRACELINK reply to indicate that the TRACE
   passed through it and where its going next.

           RPL_TRACELINK
   A TRACE reply may be composed of any number of the following numeric
   replies.

           RPL_TRACECONNECTING             RPL_TRACEHANDSHAKE
           RPL_TRACEUNKNOWN                RPL_TRACEOPERATOR
           RPL_TRACEUSER                   RPL_TRACESERVER
           RPL_TRACESERVICE                RPL_TRACENEWTYPE
           RPL_TRACECLASS

   Examples:

	TRACE *.oulu.fi                 ; TRACE to a server matching *.oulu.fi
	:WiZ TRACE AngelDust            ; TRACE issued by WiZ to nick AngelDust
*/

/*
		200     RPL_TRACELINK
                        "Link <version & debug level> <destination> \
                         <next server>"
        201     RPL_TRACECONNECTING
                        "Try. <class> <server>"
        202     RPL_TRACEHANDSHAKE
                        "H.S. <class> <server>"
        203     RPL_TRACEUNKNOWN
                        "???? <class> [<client IP address in dot form>]"

        204     RPL_TRACEOPERATOR
                        "Oper <class> <nick>"
        205     RPL_TRACEUSER
                        "User <class> <nick>"
        206     RPL_TRACESERVER
                        "Serv <class> <int>S <int>C <server> \
                         <nick!user|*!*>@<host|server>"
        208     RPL_TRACENEWTYPE
                        "<newtype> 0 <client name>"
        261     RPL_TRACELOG
                        "File <logfile> <debug level>"
*/

/*
TRACE 명령은 특정 서버에 대한 경로를 찾는 데 사용됩니다.

이 메시지를 처리하는 각 서버는 "traceroute"를 사용하여 얻은 것과 유사한 회신 체인을 형성하여 통과 링크임을 나타내는 회신을 전송하여 보낸 사람에게 알려야합니다. 
이 응답을 다시 보낸 후 주어진 서버에 도달 할 때까지 TRACE 메시지를 다음 서버로 보내야합니다. 

<server> 매개 변수가 생략 된 경우 TRACE 명령은 현재 서버가 직접 연결되어있는 서버를 알려주는 메시지를 보낸 사람에게 보내는 것이 좋습니다.
"<server>"가 지정한 대상이 실제 서버 인 경우 대상 서버는 연결된 모든 서버 및 사용자를보고해야합니다.

단, 운영자 만 사용자가있는 것을 볼 수 있습니다. <server>에 의해 제공된 목적지가 별칭이면 해당 별칭에 대한 응답 만 제공됩니다.

TRACE 메시지가 다른 서버로 향하는 경우 모든 중간 서버는 TRACE가 통과하고 다음으로 이동할 위치를 나타내는 RPL_TRACELINK 응답을 반환해야합니다.
*/

static int		get_server_count(std::map<std::string, Server *> &gs) // 직접 연결된 서버갯수 반환
{
	int		cnt;
	Server		*server;
	std::map<std::string, Server *>::iterator		iter;

	cnt = 0;
	iter = gs.begin();
	while (iter != gs.end())
	{
		server = iter->second;
		if (server->get_hopcount() == 1)
			cnt++;
		iter++;
	}
	return (cnt);
}

static int		get_client_count(std::map<std::string, Member *> &gm) // 직접 연결된 클라이언트갯수 반환
{
	int			cnt;
	Member		*member;
	std::map<std::string, Member *>::iterator		iter;

	cnt = 0;
	iter = gm.begin();
	while (iter != gm.end())
	{
		member = iter->second;
		if (member->get_socket()->get_type() == CLIENT)
			cnt++;
		iter++;
	}
	return (cnt);
}

static Server	*find_next_server(IrcServer &irc, std::string const &target_server)
{
	std::map<std::string, Server *>::iterator	iter;
	std::map<std::string, Server *>::iterator	end;
	Server										*server;
	int		target_fd = irc.find_server_fd(target_server);

	iter = irc.get_global_server().begin();
	end = irc.get_global_server().end();
	while (iter != end)
	{
		server = iter->second;
		if (target_fd == server->get_socket()->get_fd() && server->get_hopcount() == 1)
			return (server);
		iter++;
	}
	return (0);
}

static void		send_connected_server_to_socket(IrcServer &irc, Socket *socket)
{
	std::map<std::string, Server *>::iterator	iter;
	Server	*server;

	iter = irc.get_global_server().begin();
	while (iter != irc.get_global_server().end())
	{
		server = iter->second;
		if (server->get_hopcount() == 1) // 홉카운트가 1이면 직접 연결되어 있는것
		{
			socket->write(Reply(RPL::TRACESERVER(), "class", get_server_count(irc.get_global_server()), get_client_count(irc.get_global_user()),
							server->get_name(), socket->get_linkname()).get_msg().c_str());
		}
		iter++;
	}
}

void	TraceCommand::run(IrcServer &irc)
{
	Socket		*socket;
	Member		*member;
	Server		*server;
	std::string		target_server_name;

	socket = irc.get_current_socket();
	if (socket->get_type() == CLIENT)
	{
		if (_msg.get_param_size() > 1)
			throw (Reply(ERR::NEEDMOREPARAMS(), _msg.get_command()));
		if (_msg.get_param_size() == 0 || _msg.get_param(0) == irc.get_serverinfo().SERVER_NAME) // 직접 연결되어 있는 서버를 알려줌
			send_connected_server_to_socket(irc, socket);
		else // "<server>"가 지정한 대상이 실제 서버 인 경우 대상 서버는 연결된 모든 서버 및 사용자를보고해야합니다.
		{
			member = irc.find_member(socket->get_fd());
			_msg.set_prefix(member->get_nick());
			target_server_name = _msg.get_param(0);
			server = irc.get_server(target_server_name);
			if (!server)
				throw (Reply(ERR::NOSUCHSERVER(), target_server_name));
			server->get_socket()->write(_msg.get_msg());
		}
	}
	else if (socket->get_type() == SERVER)
	{
		target_server_name = _msg.get_param(0);
		if (target_server_name == irc.get_serverinfo().SERVER_NAME)
			send_connected_server_to_socket(irc, socket);
		else
		{
			server = irc.get_server(target_server_name);
			server->get_socket()->write(_msg.get_msg());
			socket->write(Reply(RPL::TRACELINK(), irc.get_serverinfo().VERSION, target_server_name, find_next_server(irc, target_server_name)->get_name()).get_msg().c_str());
		}
	}
	else if (socket->get_type() == UNKNOWN)
		throw (Reply(ERR::NOTREGISTERED()));
}

TraceCommand::TraceCommand() : Command()
{
}

TraceCommand::~TraceCommand()
{
}