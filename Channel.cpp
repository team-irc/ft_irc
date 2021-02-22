#include "Channel.hpp"

Channel::Channel(): _name(), _member()
{
};

Channel::Channel(const Channel & other): _name(other._name), _member(other._member)
{
};

Channel & Channel::operator = (const Channel & other)
{
	_member = other._member;
};

Channel::~Channel()
{
	_member.clear();
};

void Channel::add_member(const Member member)
{
	_member.push_back(member);
};