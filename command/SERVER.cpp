#include "SERVER.hpp"

SERVER::SERVER()
{
	// SERVER <servername> <hopcount> <token> <info>
}

SERVER::~SERVER()
{
}

void	SERVER::run(IrcServer& irc) 
{
	Socket		*target;

	target = irc.get_current_socket();
	irc.get_socket_set().remove_socket(target);
	target->set_type(SockType::SERVER);
	irc.get_socket_set().add_socket(target);
}