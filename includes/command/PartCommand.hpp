#ifndef PART_COMMAND_HPP
# define PART_COMMAND_HPP

# include "Command.hpp"

/*
	Command: PART
   Parameters: <channel>{,<channel>}
   The PART message causes the client sending the message to be removed
   from the list of active users for all given channels listed in the
   parameter string.

   Numeric Replies:

           ERR_NEEDMOREPARAMS              ERR_NOSUCHCHANNEL
           ERR_NOTONCHANNEL

   Examples:

   PART #twilight_zone             ; leave channel "#twilight_zone"

   PART #oz-ops,&group5            ; leave both channels "&group5" and
                                   "#oz-ops".
*/

class PartCommand : public Command
{
private:

public:
	PartCommand();
	~PartCommand();
public:
	void	run(IrcServer &irc);
};

#endif