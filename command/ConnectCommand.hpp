#ifndef CONNECT_COMMAND_HPP
# define CONNECT_COMMAND_HPP

# include "Command.hpp"

class ConnectCommand: public Command
{
public:
    ConnectCommand();
    ~ConnectCommand();
public:
    void run(IrcServer &irc);
private:
    void connect_to_target(IrcServer & irc, const std::string & target, const std::string & port);
};

#endif