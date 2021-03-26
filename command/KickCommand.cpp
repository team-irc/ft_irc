#include "KickCommand.hpp"


void KickCommand::run(IrcServer &irc)
{
	Socket		*socket;
	Channel		*channel;
	Member		*member;
	std::string	comment;

	socket = irc.get_current_socket();
	if (socket->get_type() == SOCKET_TYPE::UNKNOWN)
		return ; //err
	if (_msg.get_param_size() < 2)
		return (socket->write(Reply(ERR::NEEDMOREPARAMS).get_msg()));

	if (!(channel = irc.get_channel(_msg.get_param(0))))
		return (socket->write(Reply(ERR::NOSUCHANNEL).get_msg()));

	if (!(member = irc.get_member(_msg.get_param(1))))
		return ;

	if (_msg.get_param_size() == 3)
		comment = _msg.get_param(2);
	else
		comment = member->get_nick() + "has kicked";
	channel->delete_member(member);
	socket->write(comment.c_str());
}

KickCommand::KickCommand(): Command()
{
}

KickCommand::~KickCommand()
{
}