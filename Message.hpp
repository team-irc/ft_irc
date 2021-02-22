#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <string>
#include <vector>
#include "Channel.hpp"

#define	BUFFER_SIZE		510

class Message
{
private:
	std::string		_dest;
	char			msg[BUFFER_SIZE];
	// 거쳐온 경로
	std::vector<std::string>	_paths;
	// 		*path;
public:
	Message();
	~Message();
	void			inputPath(std::string path);
	

};

Message::Message()
{
}

Message::~Message()
{
}

#endif