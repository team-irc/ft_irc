#ifndef ISON_COMMAND_HPP
# define ISON_COMMAND_HPP

# include "Command.hpp"

class IsonCommand: public Command
{
public:
    IsonCommand();
    ~IsonCommand();
public:
    void run(IrcServer &irc);
};

#endif