#include "Command.hpp"
#include "Reply.hpp"

Command::Command() : _msg()
{
}

Command::Command(const Command &ref) : _msg(ref._msg)
{
}

Command	&Command::operator=(const Command &ref)
{
	if (this == &ref) return (*this);
	this->_msg = ref._msg;
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
	try
	{
		run(irc);
		Member * member = irc.find_member(irc.get_current_socket()->get_fd());
		std::cout << _msg.get_command() << " Command executed" << std::endl;
	}
	catch (const Reply &rpl)
	{
		irc.get_current_socket()->write(rpl.get_msg().c_str());
	}
}