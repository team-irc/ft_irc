#include "KickCommand.hpp"

void KickCommand::run(IrcServer &irc)
{
	Socket		*socket;
	Channel		*channel;
	Member		*member;
	std::string	comment;

	socket = irc.get_current_socket();

	if (_msg.get_param_size() < 2)
		socket->write(Reply(ERR_NEEDMOREPARAMS).get_msg());

	if (irc._global_channel.find(_msg.get_param(0)) == _global_channel.end())
		socket->write(Reply(ERR_NOSUCHANNEL).get_msg());

	channel = irc._global_channel.find(_msg.get_param(0)).second;
	member = irc._global_user.find(_msg.get_param(1)).second;
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