#ifndef USERHOST_COMMAND_HPP
# define USERHOST_COMMAND_HPP

# include "Command.hpp"
# include "Member.hpp"

class UserhostCommand: public Command
{
public:
    UserhostCommand();
    ~UserhostCommand();
public:
    void run(IrcServer &irc);
};

#endif