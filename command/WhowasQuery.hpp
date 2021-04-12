#ifndef WHOWAS_QUERY_HPP
# define WHOWAS_QUERY_HPP

# include "Command.hpp"

class WhowasQuery: public Command
{
public:
    WhowasQuery();
    ~WhowasQuery();
public:
    void run(IrcServer &irc);
private:
    void run(IrcServer &irc, const std::string & nickname);
    void run(IrcServer &irc, const std::string & nickname, int count);
    void run(IrcServer &irc, const std::string & nickname, const std::string & server);
    void run(IrcServer &irc, const std::string & nickname, int count, const std::string & server);
private:
    bool was_no_such_nick(IrcServer &irc, const std::string & nickname);
};

#endif