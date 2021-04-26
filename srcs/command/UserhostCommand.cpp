#include "UserhostCommand.hpp"

/*

5.7 Userhost message

      Command: USERHOST
   Parameters: <nickname>{<space><nickname>}

   The USERHOST command takes a list of up to 5 nicknames, each
   separated by a space character and returns a list of information
   about each nickname that it found.  The returned list has each reply
   separated by a space.

   Numeric Replies:

           RPL_USERHOST                    ERR_NEEDMOREPARAMS

   Examples:

   USERHOST Wiz Michael Marty p    ;USERHOST request for information on
                                   nicks "Wiz", "Michael", "Marty" and "p"

*/

void UserhostCommand::run(IrcServer &irc)
{
    int     param_size = _msg.get_param_size();
    Socket  *socket = irc.get_current_socket();
    std::vector<Member *> list;

    if (socket->get_type() == UNKNOWN)
		throw (Reply(ERR::NOTREGISTERED()));
    if (param_size <= 0)
        throw (Reply(ERR::NEEDMOREPARAMS(), "USERHOST"));
    for (int i = 0; i < param_size; ++i)
    {
        list.push_back(irc.get_member(_msg.get_param(i)));
    }
    socket->write(Reply(RPL::USERHOST(), list));
}

UserhostCommand::UserhostCommand(): Command()
{
}

UserhostCommand::~UserhostCommand()
{
}