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
		Command(const Command &ref);
		Command &operator=(const Command &ref);
		virtual void run(IrcServer& irc) = 0;
		void	set_message(const Message &msg);
		virtual ~Command();
};

#endif