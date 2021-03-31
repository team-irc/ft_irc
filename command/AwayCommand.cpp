#include "AwayCommand.hpp"

void AwayCommand::run(IrcServer &irc)
{
    Member * member = irc.find_member(irc.get_current_socket()->get_fd());

    if (_msg.get_param_size == 0)
        member->set_away(std::string());
    if (_msg.get_param_size == 1)
        member->set_away(_msg.get_param(0));
}

AwayCommand::AwayCommand(): Command()
{
}