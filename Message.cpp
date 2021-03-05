# include "Message.hpp"

Message::Message() :
	_prefix(NULL), _command(NULL), _param(), _size(0)
{}

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
			param = arr[idx];
			if (param.at(0) == ':')
			{
				idx++;
				for (; idx < size; idx++)
				{
					param += " ";
					param += arr[idx];
				}
			}
			_param.push_back(param);
			idx++;
		}
		delete[] arr;
	}
}

Message::~Message()
{
}

const int	Message::get_size()
{ return (_size); }

const std::string &Message::get_command() const
{ return (_command); }

void		Message::get_info()
{
	std::cout << "Prefix: " << _prefix << std::endl;
	std::cout << "Command: " << _command << std::endl;
	std::vector<std::string>::iterator begin = _param.begin();
	std::vector<std::string>::iterator end = _param.end();
	std::cout << "param:";
	while (begin != end)
	{
		std::cout << " " << *begin;
		begin++;
	}
	std::cout << std::endl;
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
	std::string	*param;
	int			param_size = 0;
	int			idx = 0;

	// // 없으면 걍 아무것도 안하면 됨
	// if (!_prefix.empty())
	// 	idx = insert_string(idx, _prefix);
	// // prefix, command, param 사이는 공백 하나로 구분함
	// if (idx > 0)
	// 	_msg[idx++] = ' ';
	// // 510 이상인 경우 에러
	// idx = insert_string(idx, _command);
	// // 15개 넘어가는 경우 / 510 이상인 경우 에러
	// if (idx > 509)
	// 	throw (Error("msg out of range"));
	// _msg[idx++] = ' ';
	// // 파라미터 최대 15개
	// // if (_param.size() > 15)
	// // 	throw (Error("parameter size out of range"));
	// idx = insert_string(idx, _param);
	// if (idx > 509)
	// 	throw (Error("msg out of range"));
	// _msg[idx++] = 13;
	// _msg[idx++] = 10;
	// _size = idx;
	// return (_msg);
	_msg.clear();
	if (!_prefix.empty())
		_msg += _prefix + " ";
	_msg += _command;
	if (_param.size() > 15)
		throw (Error("msg out of range"));
	std::vector<std::string>::iterator begin = _param.begin();
	std::vector<std::string>::iterator end = _param.end();
	while (begin != end)
	{
		_msg += " ";
		_msg += *begin;
		begin++;
	}
	if (_msg.length() > 510)
		throw (Error("msg out of range"));
	// _msg += 13;
	// _msg += 10;
	_size = _msg.length();
	return (_msg.c_str());
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