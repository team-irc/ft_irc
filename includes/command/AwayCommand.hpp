#ifndef AWAY_COMMAND_HPP
# define AWAY_COMMAND_HPP

# include "Command.hpp"

class AwayCommand: public Command
{
public:
    AwayCommand();
    ~AwayCommand();
public:
    void run(IrcServer &irc);
};

#endif