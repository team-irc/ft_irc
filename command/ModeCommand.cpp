# include "ModeCommand.hpp"

ModeCommand::ModeCommand()
{}

ModeCommand::ModeCommand(const ModeCommand &ref)
{}

ModeCommand::~ModeCommand()
{}

ModeCommand	&ModeCommand::operator=(const ModeCommand &ref)
{}

std::string	ModeCommand::parse_user_mode(Member *member, IrcServer &irc, char mode, mode_set set)
{
	std::string		result;
	char			new_mode = member->get_mode();
	
	// Member의 mode에 bit 연산 수행
	// 이미 설정이 된 상태라면 무시
	// Member의 mode는 1byte char, 8 = i, 4 = w, 2 = s, 1 = o로 사용
	if (mode == 'i' && member->check_mode(mode, set.is_set))
	{
		new_mode = new_mode ^ 8;
		if (set.mode != NONE && set.is_set)
			result += '+';
		else if (set.mode != NONE && !set.is_set)
			result += '-';
		result += mode;
		member->set_mode(new_mode);
	}
	else if (mode == 'w' && member->check_mode(mode, set.is_set)))
	{
		new_mode = new_mode ^ 4;
		if (set.mode != NONE && set.is_set)
			result += '+';
		else if (set.mode != NONE && !set.is_set)
			result += '-';
		result += mode;
		member->set_mode(new_mode);
	}
	else if (mode == 's' && member->check_mode(mode, set.is_set)))
	{
		new_mode = new_mode ^ 2;
		if (set.mode != NONE && set.is_set)
			result += '+';
		else if (set.mode != NONE && !set.is_set)
			result += '-';
		result += mode;
		member->set_mode(new_mode);
	}
	else if (mode == 'o' && member->check_mode(mode, set.is_set)))
	{
		new_mode = new_mode ^ 1;
		if (set.mode != NONE && set.is_set)
			result += '+';
		else if (set.mode != NONE && !set.is_set)
			result += '-';
		result += mode;
		member->set_mode(new_mode);
	}
	else
	{
		// error msg
	}
	return (result);
}

void	ModeCommand::run(IrcServer &irc)
{
	// _msg의 param을 분석해야 함
	// - + 중복 허용됨
	std::string		param = _msg.get_param(0);
	std::string		result;
	mode_set		set;
	Member			*member;
	
	set.mode = PLUS;
	set.is_set = true;
	member = irc.get_local_user(_msg.get_source_fd());
	for (int i = 0; i < param.length(); i++)
	{
		if (param.at(i) == '-')
		{
			set.mode = MINUS;
			set.is_set = false;
		}
		else if (param.at(i) == '+')
		{
			set.mode = PLUS;
			set.is_set = true;
		}
		else
		{
			if (set.is_set)
			{
				result += parse_mode(member, irc, param.at(i), set);
				set.mode = NONE;
			}
			else
			{
				result += parse_mode(member, irc, param.at(i), set);
				set.mode = NONE;
			}
		}
	}
}