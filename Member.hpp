#ifndef MEMBER_HPP
#define MEMBER_HPP

#include <string>

class Member
{
private:
	std::string		_name;
	int				_port;
public:
	Member();
	Member(const Member & other);
	Member(const std::string & name);
	Member & operator = (const Member & other);
	virtual ~Member();
public:
	const std::string & get_name();
};

#endif