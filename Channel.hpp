#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include "Member.hpp"

class Channel
{
private:
	const std::string				_name;
	std::string						_key;
	std::vector<Member>				_member;
	std::vector<char>				_mode;
	//								_properties.op_members;
	//								_properties.create_member;
	//								...
	//								_properties.mode;
	
public:
	Channel(const std::string channel_name, const std::string key, const Member &first_member);
	Channel(const std::string channel_name, const Member &first_member);
	Channel(const Channel & other);
	Channel & operator = (const Channel & other);
	~Channel();
public:
	void add_member(const Member member);
	
};

#endif