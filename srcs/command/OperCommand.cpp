#include "OperCommand.hpp"

// 1. 에러처리 필요
// 1-1. 패스워드 불일치
// 1-2. Oper 설정 안된 경우
// 1-3. Parameter 개수 부족 or 많음
// 2. 패스워드 체크 부분 필요
void	OperCommand::run(IrcServer &irc)
{
	std::string		oper_id;
	std::string		oper_pwd;
	Member			*member;
	Socket			*socket;
	

	socket = irc.get_current_socket();
	if (socket->get_type() == UNKNOWN)
		throw (Reply(ERR::NOTREGISTERED()));
	else if (socket->get_type() == SERVER)
		return ;
	if (_msg.get_param_size() != 2)
		throw (Reply(ERR::NEEDMOREPARAMS(), _msg.get_command()));
	member = irc.find_member(socket->get_fd());
	oper_id = _msg.get_param(0);
	oper_pwd = _msg.get_param(1);
	// if (!member)
	// 	throw (Reply(ERR::NOOPERHOST()));
	// 비밀번호 체크
	if (irc.check_oper(oper_id, oper_pwd))
	{
		// 체크 성공
		// MODE 설정
		// MODE member +o 전송
		char	mode = member->get_mode();
		mode = mode ^ 1;
		member->set_mode(mode);
		std::string	msg = ":" + irc.get_serverinfo().SERVER_NAME + " MODE " + member->get_nick() + " +o\n";
		irc.send_msg(socket->get_fd(), msg.c_str());
		irc.send_msg_server(socket->get_fd(), msg.c_str());
		throw (Reply(RPL::YOUREOPER()));
	}
	throw (Reply(ERR::PASSWDMISMATCH()));
}

OperCommand::OperCommand() : Command()
{
}

OperCommand::~OperCommand()
{
}