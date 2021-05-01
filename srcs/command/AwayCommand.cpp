#include "AwayCommand.hpp"

void AwayCommand::run(IrcServer &irc)
{
    Socket * socket = irc.get_current_socket();

	if (socket->get_type() == UNKNOWN)
		throw (Reply(ERR::NOTREGISTERED()));
    if (socket->get_type() == CLIENT)
    {
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
        _msg.set_prefix(member->get_nick());
        irc.send_msg_server(socket->get_fd(), _msg.get_msg());
    }
    if (socket->get_type() == SERVER)
    { // :nick AWAY or :ncik AWAY :asdf
        Member * member = irc.get_member(_msg.get_prefix());
        if (_msg.get_param_size() == 0)
        {
            member->set_away(std::string());
        }
        if (_msg.get_param_size() == 1)
        {
            member->set_away(_msg.get_param(0));
        }
        irc.send_msg_server(socket->get_fd(), _msg.get_msg());
    }
    if (socket->get_type() == UNKNOWN)
		throw(Reply(ERR::NOTREGISTERED()));
}

AwayCommand::AwayCommand(): Command()
{
}

AwayCommand::~AwayCommand()
{
}