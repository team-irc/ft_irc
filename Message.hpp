#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <string>
#include <vector>
#include "Error.hpp"
#include "Channel.hpp"

#define	BUFFER_SIZE		512

class Message
{
private:
	std::string		_dest;

	// CR(13)-LF(10) 으로 끝나야 함
	char			_msg[BUFFER_SIZE];
	// 거쳐온 경로
	std::vector<std::string>	_paths;
	// 		*path;
	std::string					_prefix;
	// command는 3자리 숫자거나 유효한 IRC 명령
	std::string					_command;
	//param은 최대 15개
	std::vector<std::string>	_param;
public:
	Message();
	~Message();
	const char		*get_msg();
	void			inputPath(std::string path);
private:
	int				insert_prefix(int start);
	int				insert_param(int start);
	int				insert_string(int start, std::string str);

};

Message::Message()
{
}

Message::~Message()
{
}

const char	*Message::get_msg()
{
	int		idx = 0;

	// 없으면 걍 아무것도 안하면 됨
	idx = insert_prefix(idx);
	// prefix, command, param 사이는 공백 하나로 구분함
	if (idx > 0)
		_msg[idx++] = ' ';
	// 510 이상인 경우 에러
	idx = insert_string(idx, _command);
	// 15개 넘어가는 경우 / 510 이상인 경우 에러
	if (idx > 509)
		throw (Error("msg out of range"));
	_msg[idx++] = ' ';
	if (_param.size() > 15)
		throw (Error("parameter size out of range"));
	idx = insert_param(idx);
	if (idx > 509)
		throw (Error("msg out of range"));
	_msg[idx++] = 13;
	_msg[idx] = 10;
	return (_msg);
}

int			Message::insert_prefix(int start)
{
	int		idx = 0;

	if (_prefix.length() != 0)
		_msg[idx++] = ':';
	return (insert_string(start + 1, _prefix));
}

int			Message::insert_string(int start, std::string str)
{
	int		idx = 0;

	while (idx < str.length())
	{
		_msg[idx + start] = str.at(idx);
		idx++;
	}
	return (idx);
}

int			Message::insert_param(int start)
{
	std::vector<std::string>::iterator	begin = _param.begin();
	std::vector<std::string>::iterator	end = _param.end();

	while (begin != end)
	{
		start = insert_string(start, *begin);
		_msg[start++] = ' ';
		begin++;
	}
	return (start);
}
#endif