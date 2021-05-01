#include "ListCommand.hpp"

void ListCommand::run(IrcServer &irc)
{
    const int param_size = _msg.get_param_size();

	if (irc.get_current_socket()->get_type() == UNKNOWN)
		throw (Reply(ERR::NOTREGISTERED()));
	if (irc.get_current_socket()->get_type() == CLIENT)
	{
		if (param_size == 0)
		{
			print_list(irc);
		}
		if (param_size == 1)
		{
			std::string *   channel_list;
			int             split_size;

			split_size = ft::split(_msg.get_param(0), ',', channel_list);
			print_list(irc, channel_list, split_size);
			delete[] channel_list;
		}
		if (param_size == 2)
		{
			Server * server = irc.get_server(_msg.get_param(1));
			if (!server)
				throw (Reply(ERR::NOSUCHSERVER(), _msg.get_param(1)));
			_msg.set_prefix(irc.find_member(irc.get_current_socket()->get_fd())->get_nick());
			irc.send_msg(server->get_socket()->get_fd(), _msg.get_msg());
		}
	}
	if (irc.get_current_socket()->get_type() == SERVER)
	{
		Member * member = irc.get_member(_msg.get_prefix());
		Channel * channel = irc.get_channel(_msg.get_param(0));
		if (channel == NULL)
			return ;
		if (_msg.get_param(1) == irc.get_serverinfo().SERVER_NAME)
		{
			irc.send_msg(member->get_socket()->get_fd(), Reply(RPL::LISTSTART()).get_msg().c_str());
			if ((channel->check_mode('p', true) && channel->check_mode('s', true)) || channel->find_member(member))
			{
				if (channel->get_servername() == irc.get_serverinfo().SERVER_NAME)
					irc.send_msg(member->get_socket()->get_fd(), Reply(RPL::LIST(), channel->get_name(), ft::itos(channel->get_members().size()), channel->get_topic()).get_msg().c_str());
			}
			irc.send_msg(member->get_socket()->get_fd(), Reply(RPL::LISTEND()).get_msg().c_str());
		}
		else
			irc.send_msg(irc.get_server(_msg.get_param(1))->get_socket()->get_fd(), _msg.get_msg());
	}
	if (irc.get_current_socket()->get_type() == UNKNOWN)
		throw(Reply(ERR::NOTREGISTERED()));
}

void ListCommand::print_list(IrcServer &irc)
{
	Socket										*socket			= irc.get_current_socket();
	std::map<std::string, Channel *>	        global_channel	= irc.get_global_channel();
	std::map<std::string, Channel *>::iterator  first			= global_channel.begin();
	std::map<std::string, Channel *>::iterator	last			= global_channel.end();
	Member										*current_user	= irc.find_member(socket->get_fd());

	socket->write(Reply(RPL::LISTSTART()).get_msg().c_str());
	while (first != last)
	{
		Channel *	channel = first->second;
		int			member_counter = channel->get_members().size();

		if ((channel->check_mode('p', true) && channel->check_mode('s', true)) || channel->find_member(current_user))
			socket->write(Reply(RPL::LIST(), channel->get_name(), ft::itos(member_counter), channel->get_topic()).get_msg().c_str());
		++first;
	}
	socket->write(Reply(RPL::LISTEND()).get_msg().c_str());
}

void ListCommand::print_list(IrcServer &irc, std::string *channel_list, int split_size)
{
    Socket	*socket = irc.get_current_socket();

	socket->write(Reply(RPL::LISTSTART()).get_msg().c_str());
	for (int i = 0; i < split_size; ++i)
	{
		Channel	*	channel = irc.get_channel(channel_list[i]);
		Member *	current_user = irc.find_member(socket->get_fd());
		int			member_counter;
		if (channel == NULL)
			return ; // no error reply for NamesCommand
		member_counter = channel->get_members().size();
		if ((channel->check_mode('p', true) && channel->check_mode('s', true)) || channel->find_member(current_user))
			socket->write(Reply(RPL::LIST(), channel->get_name(), ft::itos(member_counter), channel->get_topic()).get_msg().c_str());
	}
	socket->write(Reply(RPL::LISTEND()).get_msg().c_str());
}

ListCommand::ListCommand(): Command()
{
}

ListCommand::~ListCommand()
{
}