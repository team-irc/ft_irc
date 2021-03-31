#include "AwayCommand.hpp"

void AwayCommand::run(IrcServer &irc)
{
    Socket * socket = irc.get_current_socket();
    Member * member = irc.find_member(socket->get_fd());

    if (_msg.get_param_size() == 0)
    {
        socket->write(Reply(RPL::UNAWAY()).get_msg().c_str());
        member->set_away(std::string());
    }
    if (_msg.get_param_size() == 1)
    {
        socket->write(Reply(RPL::NOWAWAY()).get_msg().c_str());
        member->set_away(_msg.get_param(0));
    }
}

AwayCommand::AwayCommand(): Command()
{
}