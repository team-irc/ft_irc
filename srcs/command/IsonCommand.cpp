#include "IsonCommand.hpp"

/*

5.8 Ison message

      Command: ISON
   Parameters: <nickname>{<space><nickname>}

   The ISON command was implemented to provide  a  quick  and  efficient
   means  to get a response about whether a given nickname was currently
   on IRC. ISON only takes one (1) parameter: a space-separated list of
   nicks.  For  each  nickname in the list that is present, the server
   adds that to its reply string.  Thus the reply string may return
   empty (none  of  the given  nicks are present), an exact copy of the
   parameter string (all of them present) or as any other subset of the
   set of nicks  given  in  the parameter.  The only limit on the number
   of nicks that may be checked is that the combined length must not be
   too large as to cause the server to chop it off so it fits in 512
   characters.

   ISON is only be processed by the server local to the client sending
   the command and thus not passed onto other servers for further
   processing.

   Numeric Replies:

           RPL_ISON                ERR_NEEDMOREPARAMS

   Examples:

   ISON phone trillian WiZ jarlek Avalon Angel Monstah
                                   ; Sample ISON request for 7 nicks.

*/

void IsonCommand::run(IrcServer &irc)
{
    Socket                      *socket;
    int                         size;
    std::vector<std::string>    ret;

    socket = irc.get_current_socket();
	if (socket->get_type() == UNKNOWN)
		throw (Reply(ERR::NOTREGISTERED()));
    size = _msg.get_param_size();
    if (size <= 0)
        throw (Reply(ERR::NEEDMOREPARAMS(), "ISON"));
    if (socket->get_type() == UNKNOWN)
        throw(Reply(ERR::NOTREGISTERED()));
    for (int i = 0; i < size; ++i)
    {
        Member * member = irc.get_member(_msg.get_param(i));
        if (member)
            ret.push_back(member->get_nick());
    }
    socket->write(Reply(RPL::ISON(), ret).get_msg().c_str());
}

IsonCommand::IsonCommand(): Command()
{
}

IsonCommand::~IsonCommand()
{
}