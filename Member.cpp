#include "Member.hpp"

Member::Member(): _name()
{
};

Member::Member(const Member & other): _name(other._name)
{
};

Member::Member(const std::string & name): _name(name)
{
};

Member & Member::operator = (const Member & other)
{
	_name = other._name;
};

Member::~Member()
{
};

const std::string & Member::get_name()
{
	return (_name);
};