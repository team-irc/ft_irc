#include "Channel.hpp"

Channel::Channel(const std::string channel_name, const std::string key, Member *first_member)
	: _name(channel_name), _key(key), _topic()
{
	// MODE +o를 통해 네트워크에 새로운 운영자를 알림
	_member.push_back(first_member);
};

Channel::Channel(const std::string channel_name, Member *first_member)
	: _name(channel_name), _topic()
{
	// MODE +o를 통해 네트워크에 새로운 운영자를 알림
	_member.push_back(first_member);
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
	_member.push_back(member);
};

int	Channel::delete_member(Member *member)
{
	std::vector<Member *>::iterator	iter;

	iter = _member.begin();
	while (iter != _member.end())
	{
		if (*iter == member)
		{
			_member.erase(iter);
			return (1);
		}
		iter++;
	}
	return (0);
};

std::vector<Member *>	Channel::get_members()
{ return (_member); }

const std::string & 	Channel::get_name()
{ return (_name); }

bool Channel::find_mode(char c)
{
	std::vector<char>::iterator first = _mode.begin();
	std::vector<char>::iterator last = _mode.end();

	while (first != last)
	{
		if (*first == c)
			return (true);
		++first;
	}
	return (false);
}

bool Channel::find_member(Member * member)
{
	std::vector<Member *>::iterator first = _member.begin();
	std::vector<Member *>::iterator last = _member.end();

	while (first != last)
	{
		if ((*first)->get_nick() == member->get_nick())
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