# include "ModeCommand.hpp"
#include "ft_irc.hpp"

ModeCommand::ModeCommand()
{}

// ModeCommand::ModeCommand(const ModeCommand &ref)
// {}

ModeCommand::~ModeCommand()
{}

// ModeCommand	&ModeCommand::operator=(const ModeCommand &ref)
// {
	
// }

void		ModeCommand::check_target(IrcServer &irc)
{
	Channel			*channel = irc.get_channel(_msg.get_param(0));
	Member			*member = irc.get_member(_msg.get_param(0));
	std::string		param;
	std::string		msg;
	std::string		result;
	mode_set		set;

	set.mode = PLUS;
	set.is_set = true;
	if (_msg.get_param_size() < 2) {
		result = ":" + irc.get_servername() + " " + Reply(ERR::NEEDMOREPARAMS(), _msg.get_command()).get_msg();
		return (irc.get_current_socket()->write(result.c_str()));
	}
	param = _msg.get_param(1);
	msg = ":" + irc.get_servername() + " " + _msg.get_command() + " ";
	if (channel)
	{
		// 채널 동작
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
				result += parse_chan_mode(channel, irc, param.at(i), set);
				set.mode = NONE;
			}
		}
		if (!result.empty())
			msg += result + "\n";
	}
	else if (member)
	{
		for (int i = 0; i < param.length(); i++)
		{
			if (param.at(i) == '-')
			{
				if (set.is_set)
				{
					set.mode = MINUS;
					set.is_set = false;
				}
			}
			else if (param.at(i) == '+')
			{
				if (!set.is_set)
				{
					set.mode = PLUS;
					set.is_set = true;
				}
			}
			else
			{
				result += parse_user_mode(member, irc, param.at(i), set);
				set.mode = NONE;
			}
		}
		if (!result.empty())
			msg += result + "\n";
	}
	else
		msg = ":" + irc.get_servername() + " " + Reply(ERR::NOSUCHNICK(), _msg.get_param(0)).get_msg();
	if (!result.empty())
		irc.get_current_socket()->write(msg.c_str());
}

std::string	ModeCommand::parse_chan_mode(Channel *channel, IrcServer &irc, char mode, mode_set set)
{
	std::string		result;

	result += "test";
	return (result);
}

std::string	ModeCommand::parse_user_mode(Member *member, IrcServer &irc, char mode, mode_set set)
{
	std::string		result;
	char			new_mode = member->get_mode();
	
	// Member의 mode에 bit 연산 수행
	// 이미 설정이 된 상태라면 무시
	// Member의 mode는 1byte char, 8 = i, 4 = w, 2 = s, 1 = o로 사용
	if (mode == 'i')
	{
		if (member->check_mode(mode, set.is_set))
		{
			new_mode = new_mode ^ 8;
			if (set.mode != NONE && set.is_set)
				result += '+';
			else if (set.mode != NONE && !set.is_set)
				result += '-';
			result += mode;
			member->set_mode(new_mode);
		}
	}
	else if (mode == 'w')
	{
		if (member->check_mode(mode, set.is_set))
		{
			new_mode = new_mode ^ 4;
			if (set.mode != NONE && set.is_set)
				result += '+';
			else if (set.mode != NONE && !set.is_set)
				result += '-';
			result += mode;
			member->set_mode(new_mode);
		}
	}
	else if (mode == 's')
	{
		if (member->check_mode(mode, set.is_set))
		{
			new_mode = new_mode ^ 2;
			if (set.mode != NONE && set.is_set)
				result += '+';
			else if (set.mode != NONE && !set.is_set)
				result += '-';
			result += mode;
			member->set_mode(new_mode);
		}
	}
	else if (mode == 'o')
	{
		// +인 경우는 다른 서버에서 넘겨진 경우에만 처리 가능
		// -인 경우는 값 확인 후 설정해주거나 무시
		if (set.is_set && _msg.get_prefix().empty())
		{
			// +이고 클라이언트에서 보낸 경우(prefix가 없음)
			std::string msg = ":" + irc.get_servername() + " " + Reply(ERR::NOPRIVILEGES()).get_msg();
			irc.get_current_socket()->write(msg.c_str());
		}
		else
		{
			// -이거나 prefix가 있는 경우
			if (member->check_mode(mode, set.is_set))
			{
				new_mode = new_mode ^ 1;
				if (set.mode != NONE && !set.is_set)
					result += '-';
				else if (set.mode != NONE && set.is_set)
					result += '+';
				result += mode;
				member->set_mode(new_mode);
			}
		}
		return (result);
	}
	else
	{
		// error msg
		// :irc.example.net 501 test :Unknown mode "+p"
		std::string msg = ":" + irc.get_servername() + " " + mode + " " + Reply(ERR::UMODEUNKNOWNFLAG()).get_msg();
		irc.get_current_socket()->write(msg.c_str());
	}
	return (result);
}

void	ModeCommand::run(IrcServer &irc)
{
	// _msg의 param을 분석해야 함
	// - + 중복 허용됨
	
	if (irc.get_current_socket()->get_type() == UNKNOWN)
	{
		// error msg
		std::string	msg = ":" + irc.get_servername() + " " + Reply(ERR::NOTREGISTERED()).get_msg();
		irc.get_current_socket()->write(msg.c_str());
	}
	else
	{
		// 채널인지 유저인지 확인
		check_target(irc);
	}
}