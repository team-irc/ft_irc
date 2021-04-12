#ifndef WHOIS_QUERY_HPP
# define WHOIS_QUERY_HPP

# include "Command.hpp"

class WhoisQuery: public Command
{
public:
    WhoisQuery();
    ~WhoisQuery();
public:
    void run(IrcServer &irc);
private:
    struct one {};
    struct two {};
    void run(IrcServer &irc, one);
    void run(IrcServer &irc, two);
};

#endif