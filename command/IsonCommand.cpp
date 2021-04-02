#include "IsonCommand.hpp"

void IsonCommand::run(IrcServer &irc)
{
    Socket                      *socket;
    int                         size;
    std::vector<std::string>    ret;

    socket = irc.get_current_socket();
    size = _msg.get_param_size();
    if (size <= 0)
        throw (Reply(ERR::NEEDMOREPARAMS(), "ISON"));
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