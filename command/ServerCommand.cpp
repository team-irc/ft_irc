#include "ServerCommand.hpp"
#include "ft_irc.hpp"

/*
	Command: SERVER
Parameters: <servername> <hopcount> <info>

The server message is used to tell a server that the other end of a
new connection is a server. This message is also used to pass server
data over whole net.  When a new server is connected to net,
information about it be broadcast to the whole network.  <hopcount>
is used to give all servers some internal information on how far away
all servers are.  With a full server list, it would be possible to
construct a map of the entire server tree, but hostmasks prevent this
from being done.

The SERVER message must only be accepted from either (a) a connection
which is yet to be registered and is attempting to register as a
server, or (b) an existing connection to another server, in  which
case the SERVER message is introducing a new server behind that
server.

Most errors that occur with the receipt of a SERVER command result in
the connection being terminated by the destination host (target
SERVER).  Error replies are usually sent using the "ERROR" command
rather than the numeric since the ERROR command has several useful
properties which make it useful here.

If a SERVER message is parsed and attempts to introduce a server
which is already known to the receiving server, the connection from
which that message must be closed (following the correct procedures),
since a duplicate route to a server has formed and the acyclic nature
of the IRC tree broken.

Numeric Replies:

		ERR_ALREADYREGISTRED
*/

void	ServerCommand::run(IrcServer &irc)
{
	SocketSet	&ss = irc.get_socket_set();
	Socket		*socket = irc.get_current_socket();
	Server		*new_server;


	if (socket->get_type() == CLIENT)
	{
		throw (Reply(ERR::UNKNOWNCOMMAND(), "SERVER"));
	}
	if (irc.get_fd_map().find(_msg.get_param(0)) != irc.get_fd_map().end())
		throw (Reply(ERR::ALREADYREGISTRED()));
	if (socket->get_type() == UNKNOWN) // 새로운 서버 추가요청을 받은경우 (패스워드 확인 필요)
	{
		if (socket->get_pass().empty())
		{
			// :irc.example.net 461 * SERVER :Syntax error
			std::string msg = irc.get_servername();
			msg += " 461 * SERVER :Syntax error\n";
			socket->write(msg.c_str());
		}
		else if (irc.check_pass(socket))
		{
			_msg.set_param_at(1, "0"); // 1. 홉카운트 설정
			_msg.set_prefix(irc.get_servername());
			irc.send_msg_server(socket->get_fd(), _msg.get_origin()); // 2. 다른 서버에 메세지 전파
			ss.change_socket_type(_msg.get_source_fd(), SERVER); // 3. 소켓 타입 변경
			irc.send_map_data(socket->get_fd()); // 4. 맵 데이터 전송
			irc.send_user_data(socket->get_fd()); // 5. 유저 데이터 전송
			irc.add_fd_map(_msg.get_param(0).c_str(), socket->get_fd()); // 6. fd_map에 추가.
			irc.add_server(_msg.get_param(0), _msg.get_param(1), _msg.get_param(2), socket); // 7. _global_server에 추가
		}
		else
		{
			irc.get_current_socket()->write("ERROR :Bad password\n");
		}
	}
	else if (socket->get_type() == SERVER) // 다른 서버에서 서버가 추가되었음을 알리는 경우 (패스워드 확인 필요 없음)
	{
		int hopcount = ft::atoi(_msg.get_param(1).c_str());
		hopcount++;
		_msg.set_param_at(1, std::to_string(hopcount));
		irc.send_msg_server(socket->get_fd(), _msg.get_msg());
		irc.add_fd_map(_msg.get_param(0).c_str(), socket->get_fd());
		irc.add_server(_msg.get_param(0), _msg.get_param(1), _msg.get_param(2), socket);
	}
}

ServerCommand::ServerCommand() : Command()
{
}

ServerCommand::~ServerCommand()
{
}

ServerCommand::ServerCommand(ServerCommand const &copy)
{
	_msg = copy._msg;
}

ServerCommand	&ServerCommand::operator=(ServerCommand const &ref)
{
	_msg = ref._msg;
	return (*this);
}