#include "KickCommand.hpp"

void KickCommand::kick_notification(const std::string & comment, Channel * channel, IrcServer & irc)
{
	std::vector<ChanMember>	members = channel->get_members();
	std::vector<ChanMember>::iterator begin = members.begin();
	std::vector<ChanMember>::iterator end = members.end();

	while (begin != end)
	{
		irc.get_socket_set().find_socket(begin->_member->get_fd())->write(comment.c_str());
		++begin;
	}
}


void KickCommand::run(IrcServer &irc)
{
	Socket		*socket;
	Channel		*channel;
	Member		*member;
	std::string	comment;

	socket = irc.get_current_socket();
	if (socket->get_type() == UNKNOWN)
		return ; //err
	if (_msg.get_param_size() < 2)
		throw (Reply(ERR::NEEDMOREPARAMS(), "KICK"));
	if (socket->get_type() == CLIENT)
	{
		if (!(channel = irc.get_channel(_msg.get_param(0))))
			throw (Reply(ERR::NOSUCHCHANNEL(), _msg.get_param(0)));

		if (!(member = irc.get_member(_msg.get_param(1))))
			throw (Reply(ERR::NOTONCHANNEL(), _msg.get_param(1)));
		if (_msg.get_param_size() == 3)
			comment = _msg.get_param(2);
		else
			comment = member->get_nick() + "has kicked";
		kick_notification(comment, channel, irc);
		channel->delete_member(member);
	}
	else if (socket->get_type() == SERVER)
	{
	}
}

KickCommand::KickCommand(): Command()
{
}

KickCommand::~KickCommand()
{
}