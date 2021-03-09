#ifndef COMMAND_HPP
# define COMMAND_HPP

# include "SocketSet.hpp"
# include "Message.hpp"

class IrcServer;

class Command
{
	protected:
		Message		_msg;
	public:
		Command();
		Command(const Command &ref);
		Command &operator=(const Command &ref);
		virtual void run(IrcServer &irc) {};
		void	set_message(const Message &msg);
		virtual ~Command();
};

#endif