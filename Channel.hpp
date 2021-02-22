#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include "Member.hpp"

class Channel
{
private:
	const std::string				_name;
	std::vector<Member>				_member;
	//								_properties.op_members;
	//								_properties.create_member;
	//								...
	//								_properties.mode;
	
public:
	Channel();
	Channel(const Channel & other);
	Channel & operator = (const Channel & other);
	virtual ~Channel();
public:
	void add_member(const Member member);
};

#endif