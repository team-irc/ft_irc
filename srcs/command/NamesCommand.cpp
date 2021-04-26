#include "NamesCommand.hpp"

/*

4.2.5 Names message

      Command: NAMES
   Parameters: [<channel>{,<channel>}]

   By using the NAMES command, a user can list all nicknames that are
   visible to them on any channel that they can see.  Channel names
   which they can see are those which aren't private (+p) or secret (+s)
   or those which they are actually on.  The <channel> parameter
   specifies which channel(s) to return information about if valid.
   There is no error reply for bad channel names.

   If no <channel> parameter is given, a list of all channels and their
   occupants is returned.  At the end of this list, a list of users who
   are visible but either not on any channel or not on a visible channel
   are listed as being on `channel' "*".

   Numerics:

           RPL_NAMREPLY                    RPL_ENDOFNAMES

   Examples:

   NAMES #twilight_zone,#42        ; list visible users on #twilight_zone
                                   and #42 if the channels are visible to
                                   you.

   NAMES                           ; list all visible channels and users

*/

void	NamesCommand::run(IrcServer &irc)
{
	if (irc.get_current_socket()->get_type() == UNKNOWN)
		throw(Reply(ERR::NOTREGISTERED()));
	if (_msg.get_param_size() == 0)
	{
		reply_all_channel(irc);
	}
	else
	{
		reply_specific_channel(irc);
	}
}

void	NamesCommand::reply_all_channel(IrcServer & irc)
{
	Socket										*socket = irc.get_current_socket();
	std::map<std::string, Channel *>::iterator	first = irc.get_global_channel().begin();
	std::map<std::string, Channel *>::iterator	last = irc.get_global_channel().end();
	Member										*user = irc.find_member(socket->get_fd());
	Channel										*channel;

	while (first != last)
	{
		channel = first->second;
		if (channel->check_mode('p', false) || channel->check_mode('s', false))
		{
			if (channel->find_member(user))
				socket->write(Reply(RPL::NAMREPLY(), channel->get_name(), channel->get_member_list()).get_msg().c_str());
		}
		else
			socket->write(Reply(RPL::NAMREPLY(), channel->get_name(), channel->get_member_list()).get_msg().c_str());
		++first;
	}
	if (!((get_user_list_who_not_join_any_channel(irc)).empty()))
		socket->write(Reply(RPL::NAMREPLY(), "*", get_user_list_who_not_join_any_channel(irc)).get_msg().c_str());
	socket->write(Reply(RPL::ENDOFNAMES(), "*").get_msg().c_str());
}

void	NamesCommand::reply_specific_channel(IrcServer &irc)
{
	Socket		*socket;
	Channel		*channel;
	Member		*user;
	std::string *split_ret;
	int			split_len;

	socket = irc.get_current_socket();
	user = irc.find_member(socket->get_fd());
	split_len = ft::split(_msg.get_param(0), ',', split_ret);
	for (int i = 0; i < split_len; ++i)
	{
		if (!(channel = irc.get_channel(split_ret[i])))
			continue ; // no error reply for NamesCommand
		if (channel->check_mode('p', false) || channel->check_mode('s', false))
		{
			if (channel->find_member(user))
				socket->write(Reply(RPL::NAMREPLY(), channel->get_name(), channel->get_member_list()).get_msg().c_str());
		}
		else
			socket->write(Reply(RPL::NAMREPLY(), channel->get_name(), channel->get_member_list()).get_msg().c_str());
	}
	socket->write(Reply(RPL::ENDOFNAMES(), "*").get_msg().c_str());
	delete[] split_ret;
}

NamesCommand::NamesCommand(): Command()
{
}

NamesCommand::~NamesCommand()
{
}

std::vector<std::string> NamesCommand::get_user_list_who_not_join_any_channel(IrcServer &irc)
{
	std::map<std::string, Member *>::iterator	first;
	std::map<std::string, Member *>::iterator	last;
	std::map<std::string, Member *>				global_user;
	std::vector<std::string>					ret;

	global_user = irc.get_global_user();
	first = global_user.begin();
	last = global_user.end();
	while (first != last)
	{
		Member *			member = first->second;
		std::set<Channel *>	joinned_channels = member->get_joined_channels();
		if (joinned_channels.empty())
		{
			if (member->check_mode('i', true))
				ret.push_back(member->get_nick());
		}
		++first;
	}
	return (ret);
}