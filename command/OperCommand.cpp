#include "OperCommand.hpp"
#include "ft_irc.hpp"

// 1. 에러처리 필요
// 2. 패스워드 체크 부분 필요
void	OperCommand::run(IrcServer &irc)
{
	Member			*member;
	Socket			*socket;
	std::string		password;

	socket = irc.get_current_socket();
	if (_msg.get_param_size() < 2) // 파라미터 사이즈가 적다면 에러
		socket->write(Reply(ERR_NEEDMOREPARAMS).get_msg());
	member = irc.get_member(_msg.get_param(0));
	if (!member) // 유저를 못찾은 경우 에러
		socket->write(Reply(ERR_NOOPERHOST).get_msg());
	password = _msg.get_param(1);
	if (!check_password(password)) // 패스워드가 틀린 경우 에러
		socket->write(Reply(ERR_PASSWDMISMATCH).get_msg());
	if (member->get_mode() == OPER) // 멤버가 이미 오퍼레이터인 경우 응답
		socket->write(Reply(RPL_YOUREOPER).get_msg());
	member->set_mode('o');
}

OperCommand::OperCommand() : Command()
{
}

OperCommand::~OperCommand()
{
}

OperCommand::OperCommand(OperCommand const &copy)
{
	_msg = copy._msg;
}

OperCommand	&OperCommand::operator=(OperCommand const &ref)
{
	_msg = ref._msg;
	return (*this);
}