#include "AdminCommand.hpp"

AdminCommand::AdminCommand() : Command()
{}

AdminCommand::~AdminCommand()
{}

static void		send_info(IrcServer &irc, int fd, const std::string &user)
{
	struct ServerInfo	si = irc.get_serverinfo();

	Reply reply(RPL::ADMINME(), si.SERVER_NAME);
	reply.set_servername(si.SERVER_NAME);
	reply.set_username(user);
	irc.send_msg(fd, reply.get_msg().c_str());

	reply = Reply(RPL::ADMINLOC1(), si.ADMININFO1);
	reply.set_servername(si.SERVER_NAME);
	reply.set_username(user);
	irc.send_msg(fd, reply.get_msg().c_str());

	reply = Reply(RPL::ADMINLOC2(), si.ADMININFO2);
	reply.set_servername(si.SERVER_NAME);
	reply.set_username(user);
	irc.send_msg(fd, reply.get_msg().c_str());

	reply = Reply(RPL::ADMINEMAIL(), si.ADMINEMAIL);
	reply.set_servername(si.SERVER_NAME);
	reply.set_username(user);
	irc.send_msg(fd, reply.get_msg().c_str());
}

void			AdminCommand::run(IrcServer &irc)
{
	Socket	*socket = irc.get_current_socket();
	int		server_fd;

	if (socket->get_type() == UNKNOWN)
		throw(Reply(ERR::NOTREGISTERED()));
	else if (socket->get_type() == CLIENT)
	{
		// 0. param 개수 체크
		// 0-0. param 개수가 2 이상이면 에러 리턴
		if (_msg.get_param_size() > 1)
			throw (Reply(ERR::NEEDMOREPARAMS(), _msg.get_command()));
		else if (_msg.get_param_size() == 1)
		{
			// 1개면 해당 서버가 있는지 확인
			_msg.set_prefix(irc.find_member(socket->get_fd())->get_nick());
			server_fd = irc.find_server_fd(_msg.get_param(0));
			// 0-1. 없으면 에러 리턴
			if (server_fd == 0)
				throw (Reply(ERR::NOSUCHSERVER(), _msg.get_param(0)));
			// 0-2. 있으면 해당 서버로 명령어 전송
			irc.send_msg(server_fd, _msg.get_msg());
		}
		else
			send_info(irc, socket->get_fd(),
				irc.find_member(socket->get_fd())->get_nick());
	}
	else
	{
		// 0. 서버 이름 확인
		if (_msg.get_param(0) == irc.get_serverinfo().SERVER_NAME)
			send_info(irc, irc.get_member(_msg.get_prefix())->get_fd(),
				_msg.get_prefix());
		else
		{
			// 2. 다른 서버면 해당 서버로 메시지 전송
			server_fd = irc.find_server_fd(_msg.get_param(0));
			// 해당 시점에서는 서버 검증이 완료된 상태
			irc.send_msg(server_fd, _msg.get_msg());
		}
	}
}
