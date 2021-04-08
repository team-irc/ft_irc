#include "WhoQuery.hpp"

void WhoQuery::run(IrcServer &irc)
{
    int param_size;

    param_size = _msg.get_param_size();
    if (param_size == 0)
    {
        run(irc, zero());
    }
    else if (param_size == 1 || (param_size == 2 && _msg.get_param(1)[0] != 'o'))
    {
        run(irc, one());
    }
    else if (param_size == 2 && _msg.get_param(1)[0] == 'o')
    {
        run(irc, two());
    }
}

void WhoQuery::run(IrcServer &irc, zero _)
{
    (void)_;
    Socket                                      *socket;
    std::map<std::string, Member *>             global_user;
    std::map<std::string, Member *>::iterator   first;
    std::map<std::string, Member *>::iterator   last;

    socket = irc.get_current_socket();
    global_user = irc.get_global_user();
    first = global_user.begin();
    last = global_user.end();
    while (first != last)
    {
        Member * current_member = first->second;
        Server * current_server = irc.get_server(current_member->get_servername());
        if (current_member->check_mode('i', true))
            socket->write(Reply(RPL::WHOREPLY(), current_member, current_server).get_msg().c_str());
        ++first;
    }
    socket->write(Reply(RPL::ENDOFWHO(), "*").get_msg().c_str());
}

void WhoQuery::run(IrcServer &irc, one _)
{
    (void)_;
    Socket  *socket;
    Member  *target;
    Server  *target_server;

    socket = irc.get_current_socket();
    target = irc.get_member(_msg.get_param(0));
    target_server = irc.get_server(target->get_servername());
    if (target != NULL)
    {
        if (target->check_mode('i', true))
            socket->write(Reply(RPL::WHOREPLY(), target, target_server).get_msg().c_str());
    }
    socket->write(Reply(RPL::ENDOFWHO(), "*").get_msg().c_str());
}

void WhoQuery::run(IrcServer &irc, two _)
{
        (void)_;
    Socket  *socket;
    Member  *target;
    Server  *target_server;

    socket = irc.get_current_socket();
    target = irc.get_member(_msg.get_param(0));
    target_server = irc.get_server(target->get_servername());
    if (target != NULL)
    {
        if (target->check_mode('i', true) && target->check_mode('o', false))
            socket->write(Reply(RPL::WHOREPLY(), target, target_server).get_msg().c_str());
    }
    socket->write(Reply(RPL::ENDOFWHO(), "*").get_msg().c_str());
}

WhoQuery::WhoQuery(): Command()
{
}

WhoQuery::~WhoQuery()
{
}