#include "Channel.hpp"

Channel::Channel(const std::string channel_name, const std::string key, Member *first_member)
	: _name(channel_name), _key(key), _topic()
{
	// MODE +o를 통해 네트워크에 새로운 운영자를 알림
	_member.push_back(ChanMember(first_member, true, true));
};

Channel::Channel(const std::string channel_name, Member *first_member)
	: _name(channel_name), _topic()
{
	// MODE +o를 통해 네트워크에 새로운 운영자를 알림
	_member.push_back(ChanMember(first_member, true, true));
};

Channel::Channel(const Channel & other): _name(other._name), _member(other._member), _topic(other._topic)
{
};

Channel & Channel::operator = (const Channel & other)
{
	if (this == &other) return (*this);
	_member = other._member;
	_topic = other._topic;
	return (*this);
};

Channel::~Channel()
{
	// _member.clear();
};

void Channel::add_member(Member *member)
{
	_member.push_back(ChanMember(member, false, false));
};

int	Channel::delete_member(Member *member)
{
	std::vector<ChanMember>::iterator	iter;

	iter = _member.begin();
	while (iter != _member.end())
	{
		if ((*iter)._member == member)
		{
			_member.erase(iter);
			return (1);
		}
		iter++;
	}
	return (0);
};

std::vector<ChanMember>	&Channel::get_members()
{ return (_member); }

const std::string & 	Channel::get_name()
{ return (_name); }

bool Channel::find_member(Member * member)
{
	std::vector<ChanMember>::iterator first = _member.begin();
	std::vector<ChanMember>::iterator last = _member.end();

	while (first != last)
	{
		if ((*first)._member == member)
			return (true);
		++first;
	}
	return (false);
}

bool			Channel::set_topic(std::string const &topic)
{
	// 모드 관련 명령 추가?
	_topic = topic;
	return (1);
}

std::string		Channel::get_topic()
{
	return (_topic);
}

// Channel mode: o(1024) p(512) s(256) i(128) t(64) n(32) m(16) l(8) b(4) v(2) k(1)
// o, b, v는 별도의 확인 함수 사용 필요함(is_operator, is_ban_list, is_voice_list)
bool			Channel::check_mode(char mode, bool is_set)
{
	if (mode == 'o')
	{
		if (_mode & 1024)
			return (!is_set);
		return (is_set);
	}
	else if (mode == 'p')
	{
		if (_mode & 512)
			return (!is_set);
		return (is_set);
	}
	else if (mode == 's')
	{
		if (_mode & 256)
			return (!is_set);
		return (is_set);
	}
	else if (mode == 'i')
	{
		if (_mode & 128)
			return (!is_set);
		return (is_set);
	}
	else if (mode == 't')
	{
		if (_mode & 64)
			return (!is_set);
		return (is_set);
	}
	else if (mode == 'n')
	{
		if (_mode & 32)
			return (!is_set);
		return (is_set);
	}
	else if (mode == 'm')
	{
		if (_mode & 16)
			return (!is_set);
		return (is_set);
	}
	else if (mode == 'l')
	{
		if (_mode & 8)
			return (!is_set);
		return (is_set);
	}
	else if (mode == 'b')
	{
		if (_mode & 4)
			return (!is_set);
		return (is_set);
	}
	else if (mode == 'v')
	{
		if (_mode & 2)
			return (!is_set);
		return (is_set);
	}
	else if (mode == 'k')
	{
		if (_mode & 1)
			return (!is_set);
		return (is_set);
	}
	return (false);
}

int				Channel::get_mode() { return (_mode); }
void			Channel::set_mode(int mode) { _mode = mode; }
int				Channel::get_limit() { return (_limit); }
void			Channel::set_limit(int val) { _limit = val; }

bool			Channel::is_operator(Member *member)
{
	std::vector<ChanMember>::iterator		begin = _member.begin();
	std::vector<ChanMember>::iterator		end = _member.end();

	while (begin != end)
	{
		if ((*begin)._member == member && (*begin)._is_operator)
			return (true);
		begin++;
	}
	return (false);
}

void			Channel::add_operator(Member *member)
{
	std::vector<ChanMember>::iterator		begin = _member.begin();
	std::vector<ChanMember>::iterator		end = _member.end();

	while (begin != end)
	{
		if ((*begin)._member == member)
		{
			(*begin)._is_operator = true;
			return ;
		}
		begin++;
	}
}

void			Channel::delete_operator(Member *member)
{
	std::vector<ChanMember>::iterator		begin = _member.begin();
	std::vector<ChanMember>::iterator		end = _member.end();

	while (begin != end)
	{
		if ((*begin)._member == member)
		{
			(*begin)._is_operator = false;
			return ;
		}
		begin++;
	}
}

bool			Channel::is_ban_list(std::string const &mask)
{
	std::vector<std::string>::iterator		begin = _ban_list.begin();
	std::vector<std::string>::iterator		end = _ban_list.end();

	while (begin != end)
	{
		if ((*begin) == mask)
			return (true);
		begin++;
	}
	return (false);
}

void			Channel::add_ban_list(std::string const &mask)
{ _ban_list.push_back(mask); }

void			Channel::delete_ban_list(std::string const &mask)
{
	std::vector<std::string>::iterator		begin = _ban_list.begin();
	std::vector<std::string>::iterator		end = _ban_list.end();

	while (begin != end)
	{
		if ((*begin) == mask)
		{
			_ban_list.erase(begin);
			return ;
		}
		begin++;
	}
}

std::vector<std::string>	&Channel::get_ban_list()
{ return (_ban_list); }


bool			Channel::is_voice(Member *member)
{
	std::vector<ChanMember>::iterator		begin = _member.begin();
	std::vector<ChanMember>::iterator		end = _member.end();

	while (begin != end)
	{
		if ((*begin)._member == member && (*begin)._is_voice)
			return (true);
		begin++;
	}
	return (false);
}

void			Channel::add_voice(Member *member)
{
	std::vector<ChanMember>::iterator		begin = _member.begin();
	std::vector<ChanMember>::iterator		end = _member.end();

	while (begin != end)
	{
		if ((*begin)._member == member)
		{
			(*begin)._is_voice = true;
			return ;
		}
		begin++;
	}
}

void			Channel::delete_voice(Member *member)
{
	std::vector<ChanMember>::iterator		begin = _member.begin();
	std::vector<ChanMember>::iterator		end = _member.end();

	while (begin != end)
	{
		if ((*begin)._member == member)
		{
			(*begin)._is_voice = true;
			return ;
		}
		begin++;
	}
}

void			Channel::set_key(std::string const &key)
{ _key = key; }