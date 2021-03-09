#include "Channel.hpp"

Channel::Channel(const std::string channel_name, const std::string key, const Member & first_member)
	: _name(channel_name), _key(key), _member(std::vector<Member>())
{
	_member.push_back(first_member);
};

Channel::Channel(const std::string channel_name, const Member & first_member)
	: _name(channel_name), _member(std::vector<Member>())
{
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
	_member.clear();
};

void Channel::add_member(const Member member)
{
	_member.push_back(member);
};