#include "WhoisQuery.hpp"

void WhoisQuery::run(IrcServer &irc)
{
    Server	                                    *server;
    int		                                    param_size;
    Socket *                                    socket;
    std::string *                               nickmasks;
    int                                         split_size;
    std::map<std::string, Member *>::iterator   first;
    std::map<std::string, Member *>::iterator   last;

    param_size = _msg.get_param_size();
    if (param_size == 0)
        throw (Reply(ERR::NONICKNAMEGIVEN()));
    if (param_size == 1)
    {
        split_size = ft::split(_msg.get_param(0), ',', nickmasks);
    }
    if (param_size == 2)
    {
        split_size = ft::split(_msg.get_param(1), ',', nickmasks);
        if ((server = irc.get_server(_msg.get_param(0))) == NULL)
			throw (Reply(ERR::NOSUCHSERVER(), _msg.get_param(0)));
    }
    socket = irc.get_current_socket();
    for (int i = 0; i < split_size; ++i)
    {
        first = irc.get_global_user().begin();
        last = irc.get_global_user().end();
        bool search = false;
        while (first != last)
        {
            Member * current_member = first->second;
            Server * current_server = irc.get_server(current_member->get_servername());
            if (ft::check_mask(current_member->get_nick(), nickmasks[i]))
            {
                if (param_size == 2 && current_member->get_servername() != server->get_name())
                    ;
                else
                {
                    socket->write(irc, Reply(RPL::WHOISUSER(), current_member));
                    socket->write(irc, Reply(RPL::WHOISSERVER(), current_member->get_nick(), current_server));
                    if (current_member->check_mode('o', false))
                        socket->write(irc, Reply(RPL::WHOISOPERATOR(), current_member->get_nick()));
                    time_t  current_time;
                    time(&current_time);
                    socket->write(irc, Reply(RPL::WHOISIDLE(), current_member->get_nick(),
                        difftime(current_time, current_member->get_socket()->get_last_action())));
                    socket->write(irc, Reply(RPL::ENDOFWHOIS(), current_member->get_nick()));
                    search = true;
                }
            }
            ++first;
            if (!search)
                socket->write(irc, Reply(ERR::NOSUCHNICK(), nickmasks[i]));
        }
    }
    delete[] nickmasks;
}

WhoisQuery::WhoisQuery(): Command()
{
}

WhoisQuery::~WhoisQuery()
{
}