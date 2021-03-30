#include "Command.hpp"

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
	try
	{
		run(irc);
		std::cout << _msg.get_command() << " Command executed" << std::endl;
	}
	catch (const Reply &rpl)
	{
		irc.get_current_socket()->write(rpl.get_msg().c_str());
	}
}