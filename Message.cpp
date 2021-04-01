# include "Message.hpp"

Message::Message() :
	_prefix(std::string()), _command(std::string()), _param(std::vector<std::string>()), _size(0)
{}

void remove_crlf(std::string *str)
{
	if (str->empty())
		return ;
	if (str->at(str->size() - 1) == ASCII_CONST::CR || str->at(str->size() - 1) == ASCII_CONST::LF)
		str->resize(str->size() - 1);
}

Message::Message(const char *msg)
{
	int		idx = 0;
	// msg를 받아서 prefix, command, param으로 분리
	idx = 0;
	if (msg == NULL)
		return ;
	_origin = msg;
	std::string *arr;
	int size = ft::split(msg, ' ', arr);
	if (msg[0] == ':')
	{
		_prefix = arr[idx++];
		_prefix_no_collon = _prefix.substr(1);
	}
	for (int i = 0; i < arr[idx].size(); i++)
		arr[idx][i] = std::toupper(arr[idx][i]);
	_command = arr[idx++];
	remove_crlf(&_command);
	std::string param;
	while (arr[idx].empty() == false)
	{
		param = arr[idx];
		if (param.at(0) == ':')
		{
			param = param.erase(0, 1);
			idx++;
			for (; idx < size; ++idx)
			{
				param += " ";
				param += arr[idx];
			}
		}
		remove_crlf(&param);
		_param.push_back(param);
		idx++;
	}
	delete[] arr;
}

Message::Message(const Message &ref) :
	_source_fd(ref._source_fd), _dest(ref._dest), _msg(ref._msg), _origin(ref._origin), _paths(ref._paths),
	_prefix(ref._prefix), _prefix_no_collon(ref._prefix_no_collon), _command(ref._command), _param(ref._param),
	_size(ref._size), _hopcount(ref._hopcount)
{}

Message		&Message::operator=(const Message &ref)
{
	if (this != &ref)
	{
		_source_fd = ref._source_fd;
		_dest = ref._dest;
		_msg = ref._msg;
		_origin = ref._origin;
		_paths = ref._paths;
		_prefix = ref._prefix;
		_prefix_no_collon = ref._prefix_no_collon;
		_command = ref._command;
		_param = ref._param;
		_size = ref._size;
		_hopcount = ref._hopcount;
	}
	return (*this);
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

const std::string &Message::get_param(int idx) const
{ return (_param.at(idx)); }

void		Message::set_param_at(int idx, const std::string &val)
{
	if (idx > _param.size())
		throw (Error("idx out of bounds"));
	else if (idx == _param.size())
		_param.push_back(val);
	else
		_param[idx] = val;
}

void		Message::set_prefix(const std::string &prefix)
{
	if (_prefix.empty())
	{
		_prefix = ":";
		_prefix += prefix;
		_prefix_no_collon = prefix;
	}
}

void		Message::set_source_fd(const int fd)
{
	_source_fd = fd;
}

int			Message::get_source_fd()
{
	return(_source_fd);
}

const char	*Message::get_origin() const { return (_origin.c_str()); }

const char	*Message::get_msg()
{
	std::string	*param;
	int			param_size = 0;
	int			idx = 0;

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
		// if (begin + 1 == end)
		// 	_msg += ":";
		_msg += *begin;
		begin++;
	}
	if (_msg.length() > 510)
		throw (Error("msg out of range"));
	_msg += '\n';
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

const int			Message::get_param_size() const
{ return (_param.size()); }

const std::string	&Message::get_prefix() const
{
	return (_prefix_no_collon);
}