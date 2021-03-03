#include "Member.hpp"

Member::Member(): _name()
{
};

Member::Member(const Member & other): _name(other._name)
{
};

Member::Member(const std::string & name, const int & port): _name(name), _port(port)
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

const int		&Member::get_port()
{
	return (_port);
}