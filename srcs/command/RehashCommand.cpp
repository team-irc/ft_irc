#include "RehashCommand.hpp"

void    RehashCommand::run(IrcServer & irc)
{
	Socket		*socket = irc.get_current_socket();
	Member		*member = irc.find_member(socket->get_fd());
	ReadConf	rc;

	if (!member->check_mode('o', false))
		throw (Reply(ERR::NOPRIVILEGES()));
	rc.open_file("ft_irc.conf");
	rc.read_config(irc.get_serverinfo());
	if (irc.get_serverinfo().SERVER_NAME == "${AUTO}")
<<<<<<< HEAD
		irc.get_serverinfo().SERVER_NAME = "test" + ft::itos(irc.get_listen_socket()->get_port()) + ".com";
=======
		irc.get_serverinfo().SERVER_NAME = std::string("test") + ft::itos(irc.get_listen_socket()->get_port()) + ".com";
>>>>>>> d506fb6f2a87b60f7114380e12574ada479618f0
	socket->write(Reply(RPL::REHASHING(), "ft_irc.conf").get_msg().c_str());
}

RehashCommand::RehashCommand(): Command()
{
}

RehashCommand::~RehashCommand()
{
}