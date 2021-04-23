#include "WhowasQuery.hpp"

void WhowasQuery::run(IrcServer &irc)
{
    int         param_size = _msg.get_param_size();

    if (irc.get_current_socket()->get_type() == UNKNOWN)
		throw (Reply(ERR::NOTREGISTERED()));
    if (param_size == 0)
        throw (Reply(ERR::NONICKNAMEGIVEN()));
    if (param_size == 1)
    {
        run(irc, _msg.get_param(0));
    }
    if (param_size == 2)
    {
        if (ft::isdigit(_msg.get_param(1)))
            run(irc, _msg.get_param(0), ft::atoi(_msg.get_param(1).c_str()));
        else
            run(irc, _msg.get_param(0), _msg.get_param(1));
    }
    if (param_size == 3)
    {
        run(irc, _msg.get_param(0), ft::atoi(_msg.get_param(1).c_str()), _msg.get_param(2));
    }
}

void WhowasQuery::run(IrcServer &irc, const std::string & nickname)
{
    Socket *                        socket = irc.get_current_socket();
    std::vector<Member>::reverse_iterator   first = irc.get_user_history().rbegin();
    std::vector<Member>::reverse_iterator   last = irc.get_user_history().rend();

    if (was_no_such_nick(irc, nickname))
        throw (Reply(ERR::WASNOSUCHNICK(), nickname));
    while (first != last)
    {
        Member member = *first;
        if (member.get_nick() == nickname)
        {
            socket->write(Reply(RPL::WHOWASUSER(), &member));
            socket->write(Reply(RPL::WHOISSERVER(), member.get_nick(), irc.get_server(member.get_servername())));
            socket->write(Reply(RPL::ENDOFWHOWAS(), member.get_nick()));
        }
        ++first;
    }
}

void WhowasQuery::run(IrcServer &irc, const std::string & nickname, int count)
{
    Socket *                        socket = irc.get_current_socket();
    std::vector<Member>::reverse_iterator   first = irc.get_user_history().rbegin();
    std::vector<Member>::reverse_iterator   last = irc.get_user_history().rend();

    if (was_no_such_nick(irc, nickname))
        throw (Reply(ERR::WASNOSUCHNICK(), nickname));
    while (first != last && count)
    {
        Member member = *first;
        if (member.get_nick() == nickname)
        {
            socket->write(Reply(RPL::WHOWASUSER(), &member));
            socket->write(Reply(RPL::WHOISSERVER(), member.get_nick(), irc.get_server(member.get_servername())));
            socket->write(Reply(RPL::ENDOFWHOWAS(), member.get_nick()));
        }
        ++first;
        --count;
    }
}

void WhowasQuery::run(IrcServer &irc, const std::string & nickname, const std::string & server)
{
    Socket *                        socket = irc.get_current_socket();
    std::vector<Member>::reverse_iterator   first = irc.get_user_history().rbegin();
    std::vector<Member>::reverse_iterator   last = irc.get_user_history().rend();

    if (irc.get_server(server) == NULL)
        throw (Reply(ERR::NOSUCHSERVER(), server));
    if (was_no_such_nick(irc, nickname))
        throw (Reply(ERR::WASNOSUCHNICK(), nickname));
    while (first != last)
    {
        Member member = *first;
        if (member.get_nick() == nickname && member.get_servername() == server)
        {
            socket->write(Reply(RPL::WHOWASUSER(), &member));
            socket->write(Reply(RPL::WHOISSERVER(), member.get_nick(), irc.get_server(member.get_servername())));
            socket->write(Reply(RPL::ENDOFWHOWAS(), member.get_nick()));
        }
        ++first;
    }
}

void WhowasQuery::run(IrcServer &irc, const std::string & nickname, int count, const std::string & server)
{
    Socket *                        socket = irc.get_current_socket();
    std::vector<Member>::reverse_iterator   first = irc.get_user_history().rbegin();
    std::vector<Member>::reverse_iterator   last = irc.get_user_history().rend();

    if (was_no_such_nick(irc, nickname))
        throw (Reply(ERR::WASNOSUCHNICK(), nickname));
    while (first != last)
    {
        Member member = *first;
        if (member.get_nick() == nickname && member.get_servername() == server && count)
        {
            socket->write(Reply(RPL::WHOWASUSER(), &member));
            socket->write(Reply(RPL::WHOISSERVER(), member.get_nick(), irc.get_server(member.get_servername())));
            socket->write(Reply(RPL::ENDOFWHOWAS(), member.get_nick()));
        }
        ++first;
        --count;
    }
}

bool WhowasQuery::was_no_such_nick(IrcServer &irc, const std::string & nickname)
{
    std::vector<Member>::reverse_iterator   first = irc.get_user_history().rbegin();
    std::vector<Member>::reverse_iterator   last = irc.get_user_history().rend();

    while (first != last)
    {
        if (first->get_nick() == nickname)
            return (false);
        ++first;
    }
    return (true);
}

WhowasQuery::WhowasQuery(): Command()
{
}

WhowasQuery::~WhowasQuery()
{
}