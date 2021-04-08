#include "WhoisQuery.hpp"

void WhoisQuery::run(IrcServer &irc)
{
    int param_size = _msg.get_param_size();

    if (param_size == 0)
        throw (Reply(ERR::NONICKNAMEGIVEN()));
}

WhoisQuery::WhoisQuery(): Command()
{
}

WhoisQuery::~WhoisQuery()
{
}