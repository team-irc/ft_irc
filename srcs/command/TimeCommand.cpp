#include "TimeCommand.hpp"

TimeCommand::TimeCommand() : Command()
{}

TimeCommand::~TimeCommand()
{}

void	TimeCommand::run(IrcServer &irc)
{
	Socket				*socket = irc.get_current_socket();
	int					server_fd;
	std::string			result;
	struct ServerInfo	si = irc.get_serverinfo();

	if (socket->get_type() == CLIENT)
	{
		// 0. 인자가 주어진 경우 해당 서버가 있는지 확인
		// 0-1. 있다면 해당 서버로 time 쿼리 전송
		// 1. 인자가 없다면 연결된 서버의 시간을 확인하여 쿼리 전송(391)
		if (_msg.get_param_size() > 1)
			throw (Reply(ERR::NEEDMOREPARAMS(), _msg.get_command()));
		else if (_msg.get_param_size() == 0 || _msg.get_param(0) == irc.get_serverinfo().SERVER_NAME)
		{
			// 자신의 서버 시간 쿼리 전송
			ft::get_current_time(result);
			Reply	reply = Reply(RPL::TIME(), si.SERVER_NAME, result);
			reply.set_servername(si.SERVER_NAME);
			reply.set_username(irc.find_member(socket->get_fd())->get_nick());
			irc.send_msg(socket->get_fd(), reply.get_msg().c_str());
		}
		else
		{
			// 해당 서버로 명령어 전송
			_msg.set_prefix(irc.find_member(socket->get_fd())->get_nick());
			server_fd = irc.find_server_fd(_msg.get_param(0));
			if (server_fd == 0)
				throw (Reply(ERR::NOSUCHSERVER(), _msg.get_param(0)));
			irc.send_msg(server_fd, _msg.get_msg());
		}
	}
	else if (socket->get_type() == SERVER)
	{
		// 0. 서버 이름을 확인
		if (_msg.get_param(0) == si.SERVER_NAME)
		{
			// 0-1. 자신의 서버라면 해당 유저에게 쿼리 전송
			ft::get_current_time(result);
			Reply reply = Reply(RPL::TIME(), si.SERVER_NAME, result);
			reply.set_servername(si.SERVER_NAME);
			reply.set_username(_msg.get_prefix());
			int user_fd = irc.get_member(_msg.get_prefix())->get_fd();
			irc.send_msg(user_fd, reply.get_msg().c_str());
		}
		else
		{
			// 1. 다른 서버라면 해당 서버로 메시지 전송
			server_fd = irc.find_server_fd(_msg.get_param(0));
			irc.send_msg(server_fd, _msg.get_msg());
		}
	}
	else
		throw (Reply(ERR::NOTREGISTERED()));
}