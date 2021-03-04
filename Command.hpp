#ifndef COMMAND_HPP
# define COMMAND_HPP

# include "ft_irc.hpp"
# include "Message.hpp"

class Command
{
private:
	Message		_msg;
public:
	Command();
	virtual void run(const IrcServer& irc) = 0;
	void	set_message(const Message &msg);
	virtual ~Command();
};

Command::Command() : _msg()
{
}

Command::~Command()
{
}


#endif