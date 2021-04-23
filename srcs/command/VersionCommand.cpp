#include "VersionCommand.hpp"

/*
	VERSION 메시지는 서버 프로그램의 버전을 쿼리하는 데 사용됩니다. 
	선택적 매개 변수 <server>는 클라이언트가 직접 연결되지 않은 서버 프로그램의 버전을 쿼리하는 데 사용됩니다.

    Command: VERSION
       Parameters: [<server>]
       The VERSION message is used  to  query  the  version  of  the  server
       program.  An optional parameter <server> is used to query the version
       of the server program which a client is not directly connected to.

       Numeric Replies:

               ERR_NOSUCHSERVER                RPL_VERSION

       Examples:

       :Wiz VERSION *.se               ; message from Wiz to check the version
                                       of a server matching "*.se"

       VERSION tolsun.oulu.fi          ; check the version of server
                                       "tolsun.oulu.fi".

*/

void VersionCommand::run(IrcServer &irc)
{
	Socket				*socket;
	Member				*member;
	std::string			target_server;
	struct ServerInfo	si;

	socket = irc.get_current_socket();
	si = irc.get_serverinfo();
	if (socket->get_type() == CLIENT)
	{
		member = irc.find_member(socket->get_fd());
		if (_msg.get_param_size() == 0 || si.SERVER_NAME == _msg.get_param(0)) // 직접 연결된 클라이언트라면, 바로 메세지를 보내준다.
		{
			throw(Reply(RPL::VERSION(), si.VERSION, "0", si.SERVER_NAME, ""));
		}
		// 다른 서버의 버전을 찾는다면, 그 서버의 fd로 메세지를 보낸다.
		target_server = _msg.get_param(0);
		_msg.set_prefix(member->get_nick());
		if (irc.find_server_fd(target_server) == 0)
			throw (Reply(ERR::NOSUCHSERVER(), target_server));
		irc.get_socket_set().find_socket(irc.find_server_fd(target_server))->write(_msg.get_msg());
	}
	else if (socket->get_type() == SERVER)
	{
		target_server = _msg.get_param(0);
		if (target_server == si.SERVER_NAME) // 찾고있는 서버가 나라면, 응답 메세지를 되돌려보낸다.
		{
			Reply::set_servername(si.SERVER_NAME);
			Reply::set_username(_msg.get_prefix());
			socket->write(Reply(RPL::VERSION(), si.VERSION, "0", si.SERVER_NAME, "").get_msg().c_str());
		}
		else // 찾고있는 서버가 내가 아니면, 찾고있는 서버 fd에 커맨드 메세지를 보낸다.
		{
			irc.get_socket_set().find_socket(irc.find_server_fd(target_server))->write(_msg.get_msg());
		}
	}
	else if (socket->get_type() == UNKNOWN)
		throw (Reply(ERR::NOTREGISTERED()));
}

VersionCommand::VersionCommand(): Command()
{
}

VersionCommand::~VersionCommand()
{
}