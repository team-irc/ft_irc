# include "ModeCommand.hpp"

ModeCommand::ModeCommand() : _param_idx(2)
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
	Channel				*channel = irc.get_channel(_msg.get_param(0));
	Member				*member = irc.get_member(_msg.get_param(0));
	struct ServerInfo	si = irc.get_serverinfo();
	std::string			param;
	std::string			msg;
	std::string			result;
	mode_set			set;

	set.mode = PLUS;
	set.is_set = true;
	if (_msg.get_param_size() < 2)
		throw (Reply(ERR::NEEDMOREPARAMS(), _msg.get_command()));
	param = _msg.get_param(1);
	if (param.at(0) == ':')
		param = param.substr(1);
	msg = ":" + si.SERVER_NAME + " " + _msg.get_command() + " " + _msg.get_param(0) + " ";
	if (channel)
	{
		// 채널 동작
		// 0-1. 메시지를 보낸 유저가 해당 채널에 속한 유저인지 검사
		//	- 아니면 에러 리턴(442)
		// 0-2. 메시지를 보낸 멤버가 채널 관리자인지 검사
		//	- 아니면 에러 리턴(482)
		Member *sender = irc.find_member(irc.get_current_socket()->get_fd());
		if (_msg.get_prefix().empty() && sender)
		{
			if (channel->is_member(sender) == false)
				throw (Reply(ERR::NOTONCHANNEL(), channel->get_name()));
			if (channel->is_operator(sender) == false)
				throw (Reply(ERR::CHANOPRIVSNEEDED(), channel->get_name()));
		}
		_param_idx = 2;
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
		{
			msg += result; // parameter 인자 값 추가해야 함
			for (int i = 2; i < _msg.get_param_size(); i++)
				msg += " " + _msg.get_param(i);
			msg += "\n";
			// 해당 채널에 속한 멤버들에게 메시지 전송
			channel->send_msg_to_members(msg.c_str());
		}
	}
	else if (member)
	{
		// 0. 해당 멤버가 메시지를 보낸 유저인지 검사
		//	- 아니면 에러 리턴(502)
		Member *sender = irc.find_member(irc.get_current_socket()->get_fd());
		if (_msg.get_prefix().empty() && sender)
		{
			if (sender->get_nick() != _msg.get_param(0))
				throw (Reply(ERR::USERSDONTMATCH()));
		}
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
		msg = ":" + si.SERVER_NAME + " " + Reply(ERR::NOSUCHNICK(), _msg.get_param(0)).get_msg();
	if (!result.empty())
	{
		irc.send_msg_server(irc.get_current_socket()->get_fd(), msg.c_str());
		if (_msg.get_prefix().empty() && channel == NULL)
			irc.get_current_socket()->write(msg.c_str());
	}
}

static std::string	check_mode(Member *member, char mode, mode_set set, int mask)
{
	std::string		result;
	char			new_mode = member->get_mode();

	if (member->check_mode(mode, set.is_set))
	{
		new_mode = new_mode ^ mask;
		if (set.mode != NONE && set.is_set)
			result += '+';
		else if (set.mode != NONE && !set.is_set)
			result += '-';
		result += mode;
		member->set_mode(new_mode);
	}
	return (result);
}

static std::string	check_mode(Channel *channel, char mode, mode_set set, int mask)
{
	std::string		result;
	int				new_mode = channel->get_mode();

	if (channel->check_mode(mode, set.is_set))
	{
		new_mode = new_mode ^ mask;
		if (set.mode != NONE && set.is_set)
			result += '+';
		else if (set.mode != NONE && !set.is_set)
			result += '-';
		result += mode;
		channel->set_mode(new_mode);
	}
	return (result);
}

static std::string	not_check_mode(Channel *channel, char mode, mode_set set, int mask)
{
	std::string		result;
	int				new_mode = channel->get_mode();

	if (channel->check_mode(mode, set.is_set))
	{
		new_mode = new_mode ^ mask;
		channel->set_mode(new_mode);
	}
	if (set.mode != NONE && set.is_set)
		result += '+';
	else if (set.mode != NONE && !set.is_set)
		result += '-';
	result += mode;
	return (result);
}

static bool			str_is_digit(std::string str)
{
	for (int i = 0; i < str.length(); i++)
	{
		if (!ft::isdigit(str.at(i)))
			return (false);
	}
	return (true);
}

static void			parse_ban_list(std::string input, std::string *&ret)
{
	ret = new std::string[3];
	std::string		*split;
	int				size = ft::split(input, '!', split);

	ret[0] = split[0];
	if (size == 1)
	{
		// nick!*@* 이 되는 경우
		ret[1] = "*";
		ret[2] = "*";
	}
	else
	{
		std::string	tmp = split[1];
		delete[] split;
		size = ft::split(tmp, '@', split);
		ret[1] = split[0];
		// nick!user@* 이 되는 경우
		if (size == 1)
			ret[2] = "*";
		else
			ret[2] = split[1]; // nick!user@host가 되는 경우
	}
	delete[] split;
}

// Channel mode: o(1024) p(512) s(256) i(128) t(64) n(32) m(16) l(8) b(4) v(2) k(1)
std::string	ModeCommand::parse_chan_mode(Channel *channel, IrcServer &irc, char mode, mode_set set)
{
	std::string		result;
	Member			*member = 0;

	if (mode == 'o')
	{
		// 1. 서버에 해당 유저가 있는지 검사
		// 	- 없으면 에러 리턴(No such nick or channel)
		// 2. 채널에 해당 유저가 있는지 검사
		//	- 없으면 에러 리턴(They Aren't on that channel)
		// 3. 채널 관리자 벡터에 해당 멤버 추가하고 인덱스 증가
		//	- 이미 해당 멤버가 관리자 멤버에 있다면 아무 동작 없음
		// 4. -가 들어오는 경우 채널의 관리자 벡터에서 제거함
		//	- 마찬가지로 이미 세팅이 된 상태라면 아무 동작 없음
		if (_param_idx < _msg.get_param_size())
		{
			if ((member = irc.get_member(_msg.get_param(_param_idx))))
			{
				if (channel->find_member(member))
				{
					if (set.is_set && !channel->is_operator(member))
					{
						result += not_check_mode(channel, mode, set, 1024);
						channel->add_operator(member);
					}
					else if (!set.is_set && channel->is_operator(member))
					{
						result += not_check_mode(channel, mode, set, 1024);
						channel->delete_operator(member);
					}
				}
				else
				{
					// error They aren't on that channel
					irc.get_current_socket()->write(
						Reply(ERR::USERNOTINCHANNEL(), member->get_nick(), channel->get_name()).get_msg().c_str());
				}
			}
			else
			{
				irc.get_current_socket()->write(
					Reply(ERR::NOSUCHNICK(), _msg.get_param(_param_idx)).get_msg().c_str());
			}
			_param_idx++;
		}
	}
	else if (mode == 'p')
		result += check_mode(channel, mode, set, 512);
	else if (mode == 's')
		result += check_mode(channel, mode, set, 256);
	else if (mode == 'i')
		result += check_mode(channel, mode, set, 128);
	else if (mode == 't')
		result += check_mode(channel, mode, set, 64);
	else if (mode == 'n')
		result += check_mode(channel, mode, set, 32);
	else if (mode == 'm')
		result += check_mode(channel, mode, set, 16);
	else if (mode == 'l')
	{
		// param 필요(chan limit)
		// param이 정수로 변환이 안된다면 그냥 무시하고 넘김
		// 동일한 인자를 넣어도 값은 반환 됨
		// -의 경우는 다른 플래그와 동일
		if (set.is_set)
		{
			// +는 인자를 사용함, 인자가 안 맞으면 걍 넘김
			if (_param_idx < _msg.get_param_size() && str_is_digit(_msg.get_param(_param_idx)))
			{
				result += not_check_mode(channel, mode, set, 8);
				channel->set_limit(ft::atoi(_msg.get_param(_param_idx).c_str()));
				_param_idx++;
			}
		}
		else
			result += check_mode(channel, mode, set, 8);
	}
	else if (mode == 'b')
	{
		// 1. param이 있으면 해당 마스크를 벡터에 추가
		//	- 이미 있는 멤버면 아무 동작 안함
		// 2. -의 경우도 똑같이 동작 함
		if (_param_idx < _msg.get_param_size())
		{
			// 1-1 ban_list parsing
			std::string		*split;
			std::string		ban_mask;

			parse_ban_list(_msg.get_param(_param_idx), split);
			ban_mask = split[0] + "!" + split[1] + "@" + split[2];
			if (set.is_set)
			{
				result += not_check_mode(channel, mode, set, 4);
				if (!channel->is_ban_list(ban_mask))
				{
					channel->add_ban_list(ban_mask);
					result += " " + ban_mask + "\n";
					irc.get_current_socket()->write(result.c_str());
				}
			}
			else
			{
				result += not_check_mode(channel, mode, set, 4);
				if (channel->is_ban_list(ban_mask))
				{
					channel->delete_ban_list(ban_mask);
					result += " " + ban_mask + "\n";
					irc.get_current_socket()->write(result.c_str());
				}
			}
			result.clear();
			delete[] split;
			_param_idx++;
		}
		else
		{
			// 0. param이 없다면 ban mask list를 전송
			std::vector<std::string>	ban_list = channel->get_ban_list();
			std::vector<std::string>::iterator	begin = ban_list.begin();
			std::vector<std::string>::iterator	end = ban_list.end();

			while (begin != end)
			{
				// 해당 ban 리스트를 출력
				std::string msg = channel->get_name() + " " + (*begin) + "\n";
				irc.get_current_socket()->write(msg.c_str());
				begin++;
			}
			// end of ban list 출력
			std::string msg = channel->get_name() + " " + ": End of channel ban list\n";
			irc.get_current_socket()->write(msg.c_str());
		}
	}
	else if (mode == 'v')
	{
		// param 필요(nick)
		// 추가 된다면 값이 반환되고, 안되면 반환 안됨
		if (_param_idx < _msg.get_param_size())
		{
			if ((member = irc.get_member(_msg.get_param(_param_idx))))
			{
				if (channel->find_member(member))
				{
					if (set.is_set && !channel->is_voice(member))
					{
						result += not_check_mode(channel, mode, set, 2);
						channel->add_voice(member);
					}
					else if (!set.is_set && channel->is_voice(member))
					{
						result += not_check_mode(channel, mode, set, 2);
						channel->delete_voice(member);
					}
				}
				else
				{
					// error They aren't on that channel
					irc.get_current_socket()->write(
						Reply(ERR::USERNOTINCHANNEL(), member->get_nick(), channel->get_name()).get_msg().c_str());
				}
			}
			else
			{
				irc.get_current_socket()->write(
					Reply(ERR::NOSUCHNICK(), _msg.get_param(_param_idx)).get_msg().c_str());
			}
			_param_idx++;
		}
	}
	else if (mode == 'k')
	{
		// param 필요(key)
		// 동일한 인자를 넣어도 값은 반환 됨
		if (_param_idx < _msg.get_param_size())
		{
			std::string		key = _msg.get_param(_param_idx);
			result += not_check_mode(channel, mode, set, 1);
			channel->set_key(key);
		}
	}
	else
	{
		irc.get_current_socket()->write(
			Reply(ERR::UNKNOWNMODE(), ft::itos(mode)).get_msg().c_str());
	}
	return (result);
}

std::string	ModeCommand::parse_user_mode(Member *member, IrcServer &irc, char mode, mode_set set)
{
	std::string		result;
	
	// Member의 mode에 bit 연산 수행
	// 이미 설정이 된 상태라면 무시
	// Member의 mode는 1byte char, 8 = i, 4 = w, 2 = s, 1 = o로 사용
	if (mode == 'i')
		result += check_mode(member, mode, set, 8);
	else if (mode == 'w')
		result += check_mode(member, mode, set, 4);
	else if (mode == 's')
		result += check_mode(member, mode, set, 2);
	else if (mode == 'o')
	{
		// +인 경우는 다른 서버에서 넘겨진 경우에만 처리 가능
		// -인 경우는 값 확인 후 설정해주거나 무시
		if (set.is_set && _msg.get_prefix().empty())
		{
			// +이고 클라이언트에서 보낸 경우(prefix가 없음)
			irc.get_current_socket()->write(Reply(ERR::NOPRIVILEGES()).get_msg().c_str());
		}
		else
			result += check_mode(member, mode, set, 1); //-이거나 서버에서 전송 된 +인 경우
		return (result);
	}
	else
	{
		// error msg
		// :irc.example.net 501 test :Unknown mode "+p"
		irc.get_current_socket()->write(Reply(ERR::UMODEUNKNOWNFLAG()).get_msg().c_str());
	}
	return (result);
}

void	ModeCommand::run(IrcServer &irc)
{
	// _msg의 param을 분석해야 함
	// - + 중복 허용됨
	if (irc.get_current_socket()->get_type() == UNKNOWN)
		throw(Reply(ERR::NOTREGISTERED()));
	else
	{
		// 채널인지 유저인지 확인
		check_target(irc);
	}
}