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
    bool check_already_exist(IrcServer &irc, const std::string & target, const std::string & port);
    void connect_to_target(IrcServer & irc, const std::string & target, const std::string & port);
};

#endif