#ifndef PONGCOMMAND_HPP
# define PONGCOMMAND_HPP

# include "Command.hpp"

class PongCommand : public Command
{
	public:
		PongCommand();
		PongCommand(PongCommand const &ref);
		PongCommand &operator=(PongCommand const &ref);
		virtual ~PongCommand();
	public:
		void	run(IrcServer &irc);
};

#endif