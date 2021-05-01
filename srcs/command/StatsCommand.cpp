#include "StatsCommand.hpp"

/*
	STATS 메시지는 특정 서버의 통계를 조회하는 데 사용됩니다. 
	<server> 매개 변수가 생략되면 통계의 끝 응답 만 다시 전송됩니다. 
	이 명령의 구현은 응답하는 서버에 따라 크게 달라 지지만 서버는 아래 쿼리에 설명 된대로 (또는 유사한) 정보를 제공 할 수 있어야합니다.

	쿼리는 대상 서버 (<server> 매개 변수로 제공되는 경우)에서만 확인되는 단일 문자로 제공 될 수 있으며 
	그렇지 않으면 무시되고 변경되지 않고 중간 서버에 의해 전달됩니다. 
	다음 쿼리는 현재 IRC 구현에서 찾은 쿼리이며 해당 서버에 대한 설정 정보의 상당 부분을 제공합니다.
	다른 버전에서 동일한 방식으로 지원되지 않을 수 있지만 모든 서버는 현재 사용되는 응답 형식 및 쿼리의 목적과 일치하는 STATS 
	쿼리에 유효한 응답을 제공 할 수 있어야합니다.

	현재 지원되는 쿼리는 다음과 같습니다.
	c-서버가 연결하거나 연결을 허용 할 수있는 서버 목록을 반환합니다.
	h-잎사귀로 처리되거나 허브 역할을하도록 허용 된 서버 목록을 반환합니다.
	i-서버가 클라이언트의 연결을 허용하는 호스트 목록을 반환합니다.
	k-해당 서버에 대해 금지 된 사용자 이름 / 호스트 이름 조합 목록을 반환합니다.
	l-서버의 연결 목록을 반환하여 각 연결이 설정된 시간과 해당 연결을 통한 트래픽을 각 방향에 대한 바이트 및 메시지로 표시합니다.
	m-사용 횟수가 0이 아닌 경우 서버에서 지원하는 명령 목록과 각 사용 횟수를 반환합니다.
	o-일반 클라이언트가 운영자가 될 수있는 호스트 목록을 반환합니다.
	y-서버의 구성 파일에서 Y (클래스) 행을 표시합니다.
	u-서버가 작동 한 시간을 나타내는 문자열을 반환합니다.

	Command: STATS
		Parameters: [<query> [<server>]]

		The stats message is used to query statistics of certain server.	If
		<server> parameter is omitted, only the end of stats reply is sent
		back.	The implementation of this command is highly dependent on the
		server which replies, although the server must be able to supply
		information as described by the queries below (or similar).

		A query may be given by any single letter which is only checked by
		the destination server (if given as the <server> parameter) and is
		otherwise passed on by intermediate servers, ignored and unaltered.
		The following queries are those found in the current IRC
		implementation and provide a large portion of the setup information
		for that server.	Although these may not be supported in the same way
		by other versions, all servers should be able to supply a valid reply
		to a STATS query which is consistent with the reply formats currently
		used and the purpose of the query.

		The currently supported queries are:

				c - returns a list of servers which the server may connect
					to or allow connections from;
				h - returns a list of servers which are either forced to be
					treated as leaves or allowed to act as hubs;
				i - returns a list of hosts which the server allows a client
					to connect from;
				k - returns a list of banned username/hostname combinations
					for that server;
				l - returns a list of the server's connections, showing how
					long each connection has been established and the traffic
					over that connection in bytes and messages for each
					direction;
				m - returns a list of commands supported by the server and
					the usage count for each if the usage count is non zero;
				o - returns a list of hosts from which normal clients may
					become operators;
				y - show Y (Class) lines from server's configuration file;
				u - returns a string showing how long the server has been up.

		Numeric Replies:

				ERR_NOSUCHSERVER
				RPL_STATSCLINE					RPL_STATSNLINE
				RPL_STATSILINE					RPL_STATSKLINE
				RPL_STATSQLINE					RPL_STATSLLINE
				RPL_STATSLINKINFO				RPL_STATSUPTIME
				RPL_STATSCOMMANDS				RPL_STATSOLINE
				RPL_STATSHLINE					RPL_ENDOFSTATS

		Examples:

	STATS m						 ; check the command usage for the server
									you are connected to

	:Wiz STATS c eff.org			; request by WiZ for C/N line
									information from server eff.org

*/

/*
	211	 RPL_STATSLINKINFO
					"<linkname> <sendq> <sent messages> \
					<sent bytes> <received messages> \
					<received bytes> <time open>"
	212	 RPL_STATSCOMMANDS
					"<command> <count>"
	213	 RPL_STATSCLINE
					"C <host> * <name> <port> <class>"
	214	 RPL_STATSNLINE
					"N <host> * <name> <port> <class>"
	215	 RPL_STATSILINE
					"I <host> * <host> <port> <class>"
	216	 RPL_STATSKLINE
					"K <host> * <username> <port> <class>"
	218	 RPL_STATSYLINE
					"Y <class> <ping frequency> <connect \
						frequency> <max sendq>"
	219	 RPL_ENDOFSTATS
					"<stats letter> :End of /STATS report"
	241	 RPL_STATSLLINE
					"L <hostmask> * <servername> <maxdepth>"
	242	 RPL_STATSUPTIME
					":Server Up %d days %d:%02d:%02d"
	243	 RPL_STATSOLINE
					"O <hostmask> * <name>"
	244	 RPL_STATSHLINE
					"H <hostmask> * <servername>"
*/

// c - returns a list of servers which the server may connect
//	 to or allow connections from;
// h - returns a list of servers which are either forced to be
//	 treated as leaves or allowed to act as hubs;
// i - returns a list of hosts which the server allows a client
//	 to connect from;

// k - returns a list of banned username/hostname combinations
//	 for that server;

// l - returns a list of the server's connections, showing how
//	 long each connection has been established and the traffic
//	 over that connection in bytes and messages for each
//	 direction;

// m - returns a list of commands supported by the server and
//	 the usage count for each if the usage count is non zero;

// o - returns a list of hosts from which normal clients may
//	 become operators;

// y - show Y (Class) lines from server's configuration file;

// u - returns a string showing how long the server has been up.

static void		stats_l(IrcServer &irc, Socket *socket)
{
	std::map<std::string ,Server *>::iterator		iter;
	Server											*server;
	Socket											*sock;
	time_t											current_time;
	
	iter = irc.get_global_server().begin();
	while (iter != irc.get_global_server().end())
	{
		server = iter->second;
		if (server->get_hopcount() == 1)
		{
			sock = server->get_socket();
			time(&current_time);
			socket->write(Reply(RPL::STATSLINKINFO(), server->get_name(), "0", 
					ft::itos(sock->get_sent_cnt()), ft::itos(sock->get_sent_bytes()), 
					ft::itos(sock->get_recv_cnt()), ft::itos(sock->get_recv_bytes()), 
					ft::itos(current_time - sock->get_start_time())));
		}
		iter++;
	}
	
	std::vector<Socket *> vec = irc.get_socket_set().get_connect_sockets();
	std::vector<Socket *>::iterator	iter2 = vec.begin();
	std::vector<Socket *>::iterator end = vec.end();
	while (iter2 != end)
	{
		if ((*iter2)->get_type() == CLIENT)
		{
			sock = *iter2;
			socket->write(Reply(RPL::STATSLINKINFO(), sock->get_linkname(), "0", 
					ft::itos(sock->get_sent_cnt()), ft::itos(sock->get_sent_bytes()), 
					ft::itos(sock->get_recv_cnt()), ft::itos(sock->get_recv_bytes()), 
					ft::itos(current_time - sock->get_start_time())));
		}
		iter2++;
	}
}

static void		stats_m(IrcServer &irc, Socket *socket)
{
	std::map<std::string, Command *>::iterator		iter;
	std::map<std::string, Command *>::iterator		end;
	size_t											count;

	iter = irc.get_command_factory().get_command_map().begin();
	end = irc.get_command_factory().get_command_map().end();
	while (iter != end)
	{
		count = iter->second->get_count();
		if (count != 0)
			socket->write(Reply(RPL::STATSCOMMANDS(), iter->first, ft::itos(count)));
		iter++;	
	}
}

static void		stats_u(IrcServer &irc, Socket *socket)
{
	socket->write(Reply(RPL::STATSUPTIME(), irc.get_start_time()));
}

void		StatsCommand::stats(IrcServer &irc, Socket *socket, char flag)
{
	if (flag == 'l')
		stats_l(irc, socket);
	else if (flag == 'm')
		stats_m(irc, socket);
	else if (flag == 'u')
		stats_u(irc, socket);
	socket->write(Reply(RPL::ENDOFSTATS(), _msg.get_param(0).substr(0, 1)));
}

void		StatsCommand::run(IrcServer &irc)
{
	Socket			*socket;
	char			 flag;

	socket = irc.get_current_socket();
	if (socket->get_type() == CLIENT)
	{
		if (_msg.get_param_size() == 0)
			throw (Reply(RPL::ENDOFSTATS(), "*"));
		else if (_msg.get_param_size() == 1)
		{
			stats(irc, socket, _msg.get_param(0).c_str()[0]);
		}
		else if (_msg.get_param_size() == 2)
		{
			int				fd;
			std::string		server_name;

			server_name = _msg.get_param(1);
			if (server_name == irc.get_serverinfo().SERVER_NAME)
			{
				stats(irc, socket, _msg.get_param(0).c_str()[0]);
				return ;
			}
			_msg.set_prefix(irc.find_member(socket->get_fd())->get_nick()); // 1. 닉네임으로 프리픽스 설정
			fd = irc.find_server_fd(_msg.get_param(1)); // 2. 서버찾기
			if (fd <= 0)
				throw (Reply(ERR::NOSUCHSERVER(), _msg.get_param(1)));
			irc.send_msg(fd, _msg.get_msg()); // 3. 해당 서버 fd에 전송
		}
	}
	else if (socket->get_type() == SERVER) 
	{
		// 서버에서 오는 메세지 형식은 :nick STATS <flag> <server> 형식밖에 될 수 없다.
		int				fd;
		std::string		server_name;

		server_name = _msg.get_param(1);
		if (server_name == irc.get_serverinfo().SERVER_NAME)
		{
			stats(irc, socket, _msg.get_param(0).c_str()[0]);
			return ;
		}
		fd = irc.find_server_fd(_msg.get_param(1)); // 1. 서버찾기
		irc.send_msg(fd, _msg.get_msg()); // 2. 해당 서버 fd에 전송
	}
	else if (socket->get_type() == UNKNOWN)
		throw (Reply(ERR::NOTREGISTERED()));
}

StatsCommand::StatsCommand(): Command()
{
}

StatsCommand::~StatsCommand()
{
}