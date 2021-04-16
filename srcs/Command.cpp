#include "Command.hpp"
#include "Reply.hpp"

Command::Command() : _msg(), _call_count(0)
{
}

Command::Command(const Command &ref) : _msg(ref._msg), _call_count(ref._call_count)
{
}

Command	&Command::operator=(const Command &ref)
{
	if (this == &ref) return (*this);
	this->_msg = ref._msg;
	this->_call_count = ref._call_count;
	return (*this);
}

void	Command::set_message(const Message &msg)
{
	this->_msg = msg;
}

Command::~Command()
{
}

void Command::execute(IrcServer &irc)
{
	Reply	reply;
	
	if (irc.get_current_socket()->get_type() == CLIENT)
	{
		reply.set_servername(irc.get_serverinfo().SERVER_NAME);
		reply.set_username(irc.find_member(irc.get_current_socket()->get_fd())->get_nick());
	}
	else if (irc.get_current_socket()->get_type() == SERVER && !_msg.get_prefix().empty())
	{
		reply.set_servername(irc.get_serverinfo().SERVER_NAME);
		reply.set_username(_msg.get_prefix());
	}
	try
	{
		run(irc);
		std::cout << _msg.get_command() << " Command executed" << std::endl;
		_call_count++;
	}
	catch (const Reply &rpl)
	{
		irc.get_current_socket()->write(rpl.get_msg().c_str());
	}
}

size_t		Command::get_count()
{
	return (_call_count);
}