#include "WhoQuery.hpp"

void WhoQuery::run(IrcServer &irc)
{
    int                                         param_size;
    Socket                                      *socket;
    std::map<std::string, Member *>             global_user;
    std::map<std::string, Member *>::iterator   first;
    std::map<std::string, Member *>::iterator   last;

    param_size = _msg.get_param_size();
    socket = irc.get_current_socket();
    global_user = irc.get_global_user();
    first = global_user.begin();
    last = global_user.end();
    if (socket->get_type() == UNKNOWN)
		throw (Reply(ERR::NOTREGISTERED()));
    for (; first != last; ++first)
    {
        Member * current_member = first->second;
        Server * current_server = irc.get_server(current_member->get_servername());
        if (current_member->check_mode('i', false))
            continue ;
        if (_msg.get_param_size() == 0)
            ;
        else if (param_size == 1 || (param_size == 2 && _msg.get_param(1)[0] != 'o'))
        {
            if (!ft::check_mask(current_member->get_nick(), _msg.get_param(0)))
                continue ;
        }
        else if (param_size == 2 && _msg.get_param(1)[0] == 'o')
        {
            if (!ft::check_mask(current_member->get_nick(), _msg.get_param(0)) || !current_member->check_mode('o', false))
                continue ;
        }
        socket->write(Reply(RPL::WHOREPLY(), current_member, current_server).get_msg().c_str());
    }
    if (param_size != 0 && !_msg.get_param(0).empty())
        socket->write(Reply(RPL::ENDOFWHO(), _msg.get_param(0)).get_msg().c_str());
    else
        socket->write(Reply(RPL::ENDOFWHO(), "*").get_msg().c_str());
}

WhoQuery::WhoQuery(): Command()
{
}

WhoQuery::~WhoQuery()
{
}