#include "Command.hpp"

Command::Command() : _msg()
{
}

Command::Command(const Command &ref) : _msg(ref._msg)
{
}

Command	&Command::operator=(const Command &ref)
{
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