#include "OperCommand.hpp"
#include "ft_irc.hpp"

// 1. 에러처리 필요
// 2. 패스워드 체크 부분 필요
void	OperCommand::run(IrcServer &irc)
{
	Member			*member;
	Socket			*socket;
	std::string		password;

	if (!deal_exception(irc))
		return ;
	socket = irc.get_current_socket();
	member = irc.get_member(_msg.get_param(0));
	password = _msg.get_param(1);
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

bool OperCommand::deal_exception(IrcServer &irc)
{
	Socket	*socket = irc.get_current_socket();

	if (_msg.get_param_size() < 2)
		goto ERR_NEEDMOREPARAMS;
	if (!irc.get_member(_msg.get_param(0)))
		goto ERR_NOOPERHOST;
	if (!check_password(_msg.get_param(1))) // 패스워드가 틀린 경우 에러
		goto ERR_PASSWDMISMATCH;
	return (true);

ERR_NEEDMOREPARAMS:
	socket->write((Reply(ERR::NEEDMOREPARAMS()).get_msg()).c_str())
	return (false);

ERR_NOOPERHOST:
	socket->write((Reply(ERR::NOOPERHOST()).get_msg()).c_str());
	return (false);

ERR_PASSWDMISMATCH:
	socket->write((Reply(ERR::PASSWDMISMATCH()).get_msg()).c_str());
	return (false);
}