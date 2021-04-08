#ifndef WHOIS_QUERY_HPP
# define WHOIS_QERU_HPP

# include "Command.hpp"

class WhoisQuery: public Command
{
public:
    WhoisQuery();
    ~WhoisQuery();
public:
    void run(IrcServer &irc);
};

#endif