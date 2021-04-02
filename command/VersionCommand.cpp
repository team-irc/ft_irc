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
	Socket			*socket;
	Member			*member;
	std::string		target_server;

	socket = irc.get_current_socket();
	if (socket->get_type() == CLIENT)
	{
		member = irc.find_member(socket->get_fd());
		if (!(_msg.get_param_size() <= 1))
			throw(Reply(ERR::NEEDMOREPARAMS(), _msg.get_command()));
		if (_msg.get_param_size() == 0)
		{
			throw(Reply(RPL::VERSION(), irc.get_version(), irc.get_debug_level(), irc.get_servername(), ""));
		}
		else
		{
			target_server = _msg.get_param(0);
			_msg.set_prefix(member->get_nick());

			irc.get_socket_set().find_socket(irc.find_fd_map(target_server))->write(_msg.get_msg());
		}
	}
	else if (socket->get_type() == SERVER)
	{
		target_server = _msg.get_param(0);
		if (target_server == irc.get_servername())
		{
			Reply::set_servername(irc.get_servername());
			Reply::set_username(_msg.get_prefix());
			socket->write(Reply(RPL::VERSION(), irc.get_version(), irc.get_debug_level(), irc.get_servername(), "").get_msg().c_str());
		}
		else
		{
			irc.get_socket_set().find_socket(irc.find_fd_map(target_server))->write(_msg.get_msg());
		}
	}
	else
	{
		return ;
	}
}

VersionCommand::VersionCommand(): Command()
{
}

VersionCommand::~VersionCommand()
{
}