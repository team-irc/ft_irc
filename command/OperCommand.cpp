#include "OperCommand.hpp"
#include "ft_irc.hpp"

// 1. 에러처리 필요
// 2. 패스워드 체크 부분 필요
void	OperCommand::run(IrcServer &irc)
{
	Member			*member;
	Socket			*socket;
	std::string		password;

	if (_msg.get_param_size() < 2)
		throw (Reply(ERR::NEEDMOREPARAMS()));
	socket = irc.get_current_socket();
	member = irc.get_member(_msg.get_param(0));
	password = _msg.get_param(1);
	if (!member)
		throw (Reply(ERR::NOOPERHOST()));
	// 비밀번호 체크
	if (member->get_mode() == OPER) // 멤버가 이미 오퍼레이터인 경우 응답
		socket->write((Reply(RPL::YOUREOPER()).get_msg()).c_str());
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