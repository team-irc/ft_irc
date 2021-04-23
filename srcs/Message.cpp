# include "Message.hpp"

Message::Message() :
	_prefix(std::string()), _command(std::string()), _param(std::vector<std::string>()), _size(0)
{}

static void remove_crlf(std::string *str)
{
	if (str->empty())
		return ;
	if (str->at(str->size() - 1) == ASCII_CONST::CR || str->at(str->size() - 1) == ASCII_CONST::LF)
		str->resize(str->size() - 1);
}

static int	split_for_message(const char *msg, char c, std::string *& arr)
{
	int			i = 0;
	std::string	tmp[2];
	std::string *tmp_split_ret;
	int			tmp_size;

	if (ft::strchr(msg + 1, ':') != NULL)
	{
		// 첫 번쨰 콜론 기준으로 문자열 반갈죽
		if (msg[i] == ':') ++i; // 문자열 첫번째에 있는 콜론은 무시
		while (msg[i] != ':')
			tmp[0] += msg[i++];
		while (msg[i])
			tmp[1] += msg[i++];
		tmp_size = ft::split(tmp[0].c_str(), c, tmp_split_ret);
		arr = new std::string[tmp_size + 2];
		i = 0;
		while (i < tmp_size)
		{
			arr[i] = tmp_split_ret[i];
			++i;
		}
		arr[i] = tmp[1];
		delete[] tmp_split_ret;
		return (tmp_size + 1);
	}
	else
		return (ft::split(msg, c, arr));
}

Message::Message(const char *msg)
{
	int		idx = 0;
	// msg를 받아서 prefix, command, param으로 분리
	idx = 0;
	_size = 0;
	if (msg == NULL || msg[0] == 0)
		return ;
	_origin = msg;
	std::string *arr;
	int size = split_for_message(msg, ' ', arr);
	if (msg[0] == ':')
	{
		if (arr[0][0] == ':')
			_prefix = arr[idx++].substr(1);
		else
			_prefix = arr[idx++];
		_size +=  _prefix.length() + 1;
	}
	for (int i = 0; i < arr[idx].size(); i++)
		arr[idx][i] = std::toupper(arr[idx][i]);
	_command = arr[idx++];
	_size +=  _command.length();
	remove_crlf(&_command);
	std::string param;
	while (arr[idx].empty() == false)
	{
		param = arr[idx];
		remove_crlf(&param);
		_param.push_back(param);
		_size += param.length() + 1;
		idx++;
	}
	std::cout << std::endl;
	delete[] arr;
}

Message::Message(const Message &ref) :
	_source_fd(ref._source_fd), _dest(ref._dest), _msg(ref._msg), _origin(ref._origin), _paths(ref._paths),
	_prefix(ref._prefix), _command(ref._command), _param(ref._param),
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
	if (idx == _param.size())
		_param.push_back(val);
	else
		_param[idx] = val;
}

void		Message::set_prefix(const std::string &prefix)
{
	// if (_prefix.empty())
	_prefix = prefix;
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
		_msg += ":" + _prefix + " ";
	_msg += _command;
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
	return (_prefix);
}