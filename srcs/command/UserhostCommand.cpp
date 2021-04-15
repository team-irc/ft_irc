#include "UserhostCommand.hpp"

void UserhostCommand::run(IrcServer &irc)
{
    Socket  *socket = irc.get_current_socket();
    std::vector<Member *> list;

    if (_msg.get_param_size() <= 0)
        throw (Reply(ERR::NEEDMOREPARAMS(), "USERHOST"));
    for (int i = 0; i < _msg.get_param_size(); ++i)
    {
        list.push_back(irc.get_member(_msg.get_param(i)));
    }
    socket->write(Reply(RPL::USERHOST(), list).get_msg().c_str());
}

UserhostCommand::UserhostCommand(): Command()
{
}