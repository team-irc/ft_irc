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

class Member;
class Channel;

class ModeCommand : public Command
{
	public:
		ModeCommand();
		// ModeCommand(const ModeCommand &ref);
		// ModeCommand	&operator=(const ModeCommand &ref);
		~ModeCommand();
		void	run(IrcServer &irc);
	private:
		int			_param_idx;
		void		check_target(IrcServer &irc);
		std::string	parse_user_mode(Member *member, IrcServer &irc, char mode, mode_set set);
		std::string	parse_chan_mode(Channel *channel, IrcServer &irc, char mode, mode_set set);
};

#endif