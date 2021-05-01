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
};

#endif