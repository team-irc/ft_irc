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
			socket->write(get_channel_user_list(first->second).c_str());
			socket->write("\n");
			++first;
		}
	}
	else
	{
		if (!(channel = irc.get_channel(_msg.get_param(0))))
			return ;
		socket->write(get_channel_user_list(channel).c_str());
		socket->write("\n");
	}
}

NamesCommand::NamesCommand(): Command()
{
}

std::string NamesCommand::get_channel_user_list(Channel * channel)
{
	std::string						ret;
	std::vector<Member *>			members;
	std::vector<Member *>::iterator	first;
	std::vector<Member *>::iterator	last;

	members = channel->get_members();
	first = members.begin();
	last = members.end();
	ret += "Nicks from " + channel->get_name() + ": [";
	while (first != last)
	{
		ret += (*first)->get_nick();
		if (first + 1 != last)
			ret += ", ";
		++first;
	}
	ret += " ]";
	return (ret);
}