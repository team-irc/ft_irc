# include "Message.hpp"

Message::Message(const char *msg)
{
	int		idx = 0;
	// msg를 받아서 prefix, command, param으로 분리
	if (msg != NULL)
	{
		std::string *arr;
		int size = ft::split(msg, ' ', arr);
		if (msg[0] == ':')
			_prefix = arr[idx++];
		_command = arr[idx++];
		std::string param;
		while (idx < size)
		{
			param += arr[idx];
			param += " ";
			idx++;
		}
		_param = param;
		delete[] arr;
	}
}

Message::~Message()
{
}

const int	Message::get_size()
{ return (_size); }

void		Message::get_info()
{
	std::cout << "Prefix: " << _prefix << std::endl;
	std::cout << "Command: " << _command << std::endl;
	std::cout << "Params: " << _param << std::endl;
}

void		Message::set_prefix(const char *prefix)
{
	if (prefix != NULL)
	{
		_prefix = ":";
		_prefix += prefix;
	}
}

const char	*Message::get_msg()
{
	int		idx = 0;

	// 없으면 걍 아무것도 안하면 됨
	if (!_prefix.empty())
		idx = insert_string(idx, _prefix);
	// prefix, command, param 사이는 공백 하나로 구분함
	if (idx > 0)
		_msg[idx++] = ' ';
	// 510 이상인 경우 에러
	idx = insert_string(idx, _command);
	// 15개 넘어가는 경우 / 510 이상인 경우 에러
	if (idx > 509)
		throw (Error("msg out of range"));
	_msg[idx++] = ' ';
	// 파라미터 최대 15개
	// if (_param.size() > 15)
	// 	throw (Error("parameter size out of range"));
	idx = insert_string(idx, _param);
	if (idx > 509)
		throw (Error("msg out of range"));
	_msg[idx++] = 13;
	_msg[idx] = 10;
	_size = idx + 1;
	return (_msg);
}

int			Message::insert_string(int start, std::string str)
{
	int		idx = 0;

	while (idx < str.length())
	{
		_msg[idx + start] = str.at(idx);
		idx++;
	}
	return (idx + start);
}