#ifndef WHO_COMMAND_HPP
# define WHO_COMMAND_HPP

# include "Command.hpp"

class WhoQuery: public Command
{
public:
    WhoQuery();
    ~WhoQuery();
public:
    void run(IrcServer &irc);
private:
    struct zero {};
    struct one {};
    struct two {};
    void run(IrcServer &irc, zero);
    void run(IrcServer &irc, one);
    void run(IrcServer &irc, two);
};

#endif