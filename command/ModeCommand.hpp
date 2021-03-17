#ifndef MODECOMMAND_HPP
# define MODECOMMAND_HPP
# include "Command.hpp"

enum MODE {
	PLUS, MINUS, NONE
};

struct mode_set {
	MODE	mode;
	bool	is_set;
};

class ModeCommand : public Command
{
	public:
		ModeCommand();
		ModeCommand(const ModeCommand &ref);
		ModeCommand	&operator=(const ModeCommand &ref);
		virtual ~ModeCommand();
		void	run(IrcServer &irc);
	private:
		std::string	parse_user_mode(Member *member, IrcServer &irc, char mode, mode_set set);
}

#endif