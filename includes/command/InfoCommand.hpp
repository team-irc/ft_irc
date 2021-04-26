#ifndef INFO_COMMAND_HPP
# define INFO_COMMAND_HPP

# include "Command.hpp"

class InfoCommand: public Command
{
public:
    InfoCommand();
    ~InfoCommand();
public:
    void run(IrcServer &irc);
};

#endif