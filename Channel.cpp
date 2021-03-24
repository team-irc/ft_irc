#include "Channel.hpp"

Channel::Channel(const std::string channel_name, const std::string key, Member *first_member)
	: _name(channel_name), _key(key)
{
	// MODE +o를 통해 네트워크에 새로운 운영자를 알림
	_member.push_back(first_member);
};

Channel::Channel(const std::string channel_name, Member *first_member)
	: _name(channel_name)
{
	// MODE +o를 통해 네트워크에 새로운 운영자를 알림
	_member.push_back(first_member);
};

Channel::Channel(const Channel & other): _name(other._name), _member(other._member)
{
};

Channel & Channel::operator = (const Channel & other)
{
	if (this == &other) return (*this);
	_member = other._member;
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

void Channel::delete_member(Member *member)
{
	std::vector<Member *>::iterator	first;
	std::vector<Member *>::iterator	last;

	first = _member.begin();
	last = _member.end();
	while (first != last)
	{
		if (*first == member)
			return (_member.erase(first));
		++first;
	}
	return (_member.end());
};

std::vector<Member *>	Channel::get_members()
{ return (_member); }