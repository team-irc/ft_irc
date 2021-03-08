#ifndef COMMAND_HPP
# define COMMAND_HPP

# include "SocketSet.hpp"
# include "Message.hpp"

class Command
{
	protected:
		Message		_msg;
	public:
		Command();
		Command(const Command &ref);
		Command &operator=(const Command &ref);
		virtual void run(SocketSet& socket_set) {};
		void	set_message(const Message &msg);
		virtual ~Command();
};

#endif