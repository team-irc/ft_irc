#include "WhoisQuery.hpp"

void WhoisQuery::run(IrcServer &irc)
{
    Server	*server;
    int		param_size = _msg.get_param_size();

    if (param_size == 0)
        throw (Reply(ERR::NONICKNAMEGIVEN()));
    if (param_size == 2)
        if ((server = irc.get_server(_msg.get_param(0))) == NULL)
			throw (Reply(ERR::NOSUCHSERVER(), _msg.get_param(0)));
    if (param_size == 1)
    {
        run(irc, one());
    }
    else if (param_size == 2)
    {
        run(irc, two());
    }
}

void WhoisQuery::run(IrcServer &irc, one _)
{
    (void)_;
    Socket *                                    socket;
    std::string *                               nickmasks;
    int                                         split_size;
    std::map<std::string, Member *>::iterator   first;
    std::map<std::string, Member *>::iterator   last;

    socket = irc.get_current_socket();
    split_size = ft::split(_msg.get_param(0), ',', nickmasks);
    for (int i = 0; i < split_size; ++i)
    {
        first = irc.get_global_user().begin();
        last = irc.get_global_user().end();
        while (first != last)
        {
            Member * current_member = first->second;
            Server * current_server = irc.get_server(current_member->get_servername());
            if (ft::check_mask(current_member->get_nick(), nickmasks[i]))
            {
                socket->write(Reply(RPL::WHOISUSER(), current_member));
                if (!current_member->get_joined_channels().empty())
                    socket->write(Reply(RPL::WHOISCHANNELS(), current_member, current_member->get_joined_channels()));
                socket->write(Reply(RPL::WHOISSERVER(), current_member->get_nick(), current_server));
                if (current_member->check_mode('0', false))
                    socket->write(Reply(RPL::WHOISOPERATOR(), current_member->get_nick()));
                socket->write(Reply(RPL::WHOISIDLE(), current_member->get_nick(), 0));
                socket->write(Reply(RPL::ENDOFWHOIS(), current_member->get_nick()));
            }
            ++first;
        }
    }
    delete[] nickmasks;
}

void WhoisQuery::run(IrcServer &irc, two _)
{
    (void)_;
    Socket *                                    socket;
    std::string *                               nickmasks;
    int                                         split_size;
    std::map<std::string, Member *>::iterator   first;
    std::map<std::string, Member *>::iterator   last;

    socket = irc.get_current_socket();
    split_size = ft::split(_msg.get_param(1), ',', nickmasks);
    for (int i = 0; i < split_size; ++i)
    {
        first = irc.get_global_user().begin();
        last = irc.get_global_user().end();
        while (first != last)
        {
            Member * current_member = first->second;
            Server * current_server = irc.get_server(current_member->get_servername());
            if (ft::check_mask(current_member->get_nick(), nickmasks[i]) && current_member->get_servername() == _msg.get_param(0))
            {
                socket->write(Reply(RPL::WHOISUSER(), current_member));
                socket->write(Reply(RPL::WHOISSERVER(), current_member->get_nick(), current_server));
                if (current_member->check_mode('0', false))
                    socket->write(Reply(RPL::WHOISOPERATOR(), current_member->get_nick()));
                socket->write(Reply(RPL::WHOISIDLE(), current_member->get_nick(), 0));
                socket->write(Reply(RPL::ENDOFWHOIS(), current_member->get_nick()));
            }
            ++first;
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