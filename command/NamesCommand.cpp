#include "NamesCommand.hpp"
#include "ft_irc.hpp"

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
	Socket							*socket;
	Channel							*channel;
	std::string						ret;

	socket = irc.get_current_socket();
	if (socket->get_type() == UNKNOWN)
		return ;
	if (_msg.get_param_size() == 0)
	{
		std::map<std::string, Channel *>::iterator first = irc.get_global_channel().begin();
		std::map<std::string, Channel *>::iterator last = irc.get_global_channel().end();

		while (first != last)
		{
			channel = first->second;

			if (!(channel->find_mode('p') || channel->find_mode('s')))
				socket->write(Reply(RPL::NAMREPLY(), channel->get_name(), get_channel_user_list(channel)).get_msg().c_str());
			++first;
		}
		if (!((get_user_list_who_not_join_any_channel(irc)).empty()))
			socket->write(Reply(RPL::NAMREPLY(), "*", get_user_list_who_not_join_any_channel(irc)).get_msg().c_str());
		socket->write(Reply(RPL::ENDOFNAMES(), "*").get_msg().c_str());
	}
	else
	{
		if (!(channel = irc.get_channel(_msg.get_param(0))))
			return ;
		if (!(channel->find_mode('p') || channel->find_mode('s')))
		{
			socket->write(Reply(RPL::NAMREPLY(), channel->get_name(), get_channel_user_list(channel)).get_msg().c_str());
			socket->write(Reply(RPL::ENDOFNAMES(), channel->get_name()).get_msg().c_str());
		}
	}
}

NamesCommand::NamesCommand(): Command()
{
}

bool	NamesCommand::is_he_invisible(Member * member)
{
	std::set<Channel *>				joinned_channels = member->get_joinned_channels();
	std::set<Channel *>::iterator	first = joinned_channels.begin();
	std::set<Channel *>::iterator	last = joinned_channels.end();

	while (first != last)
	{
		Channel * channel = *first;
		if (!(channel->find_mode('p') || channel->find_mode('s')))
			return (false);
		++first;
	}
	return (true);
}

std::vector<std::string> NamesCommand::get_channel_user_list(Channel * channel)
{
	std::vector<std::string>		ret;
	std::vector<Member *>			members;
	std::vector<Member *>::iterator	first;
	std::vector<Member *>::iterator	last;

	members = channel->get_members();
	first = members.begin();
	last = members.end();
	while (first != last)
	{
		ret.push_back((*first)->get_nick());
		++first;
	}
	return (ret);
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
		std::set<Channel *>	joinned_channels = member->get_joinned_channels();
		if (joinned_channels.empty() || is_he_invisible(member))
		{
			ret.push_back(member->get_nick());
		}
		++first;
	}
	return (ret);
}