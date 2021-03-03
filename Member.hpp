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
	Member(const std::string & name, const int &port);
	Member(const Member & other);
	Member & operator = (const Member & other);
	virtual ~Member();
public:
	const std::string	& get_name();
	const int			& get_port();
};

#endif