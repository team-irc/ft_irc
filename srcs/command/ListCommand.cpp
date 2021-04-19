#include "ListCommand.hpp"

void ListCommand::run(IrcServer &irc)
{
    const int param_size = _msg.get_param_size();

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