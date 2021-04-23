#include "utils.hpp"

int ft::atoi(const char * nptr)
{
	int i;
	int result_num;
	int minus;

	i = 0;
	result_num = 0;
	minus = 0;
	while (nptr[i] == ' ')
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			minus = 1;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		result_num *= 10;
		result_num += nptr[i] - '0';
		i++;
	}
	return (minus == 0 ? result_num : -result_num);
}

std::string ft::itos(int n)
{
	std::string ret;
	int			ret_begin;

	ret_begin = n < 0 ? 1 : 0;
	if (n == 0)
		ret = "0";
	if (n < 0)
	{
		ret += '-';
		n *= -1;
	}
	while (n)
	{
		// 0->48
		ret.insert(ret_begin, 1, n % 10 + 48);
		n /= 10;
	}
	return (ret);
}

std::string ft::itos(long n)
{
	std::string ret;
	int			ret_begin;

	ret_begin = n < 0 ? 1 : 0;
	if (n == 0)
		ret = "0";
	if (n < 0)
	{
		ret += '-';
		n *= -1;
	}
	while (n)
	{
		// 0->48
		ret.insert(ret_begin, 1, n % 10 + 48);
		n /= 10;
	}
	return (ret);
}

std::string ft::itos(unsigned short n)
{
	std::string ret;
	int			ret_begin = 0;

	if (n == 0)
		ret = "0";
	while (n)
	{
		// 0->48
		ret.insert(ret_begin, 1, n % 10 + 48);
		n /= 10;
	}
	return (ret);
}

std::string ft::itos(unsigned long n)
{
	std::string ret;
	int			ret_begin = 0;
	
	if (n == 0)
		ret = "0";
	while (n)
	{
		// 0->48
		ret.insert(ret_begin, 1, n % 10 + 48);
		n /= 10;
	}
	return (ret);
}

int ft::split(const std::string str, char c, std::string *& ret)
{
	int size;
	int str_counter;

	size = 0;
	str_counter = 0;
	for (int i = 0; i < str.length();)
	{
		if (str[i] == c)
		{
			++size;
			while (str[i] == c)
				++i;
			if (str[i] == 0 || str[i] == ASCII_CONST::CR || str[i] == ASCII_CONST::LF)
				--size;
		}
		else
			++i;
	}
	++size;
	ret = new std::string[size + 1];
	for (int i = 0; i < size; ++i)
	{
		for (; (str[str_counter] != c && str_counter < str.length()); ++str_counter)
			ret[i] += str[str_counter];
		while (str[str_counter] == c)
			++str_counter;
	}
	ret += 0;
	return (size);
}

char	*ft::strchr(const char *s, int c)
{
	while (*s != '\0')
	{
		if (*s == c)
			return ((char *)s);
		s++;
	}
	if (c == '\0')
		return ((char*)s);
	return (NULL);
}

bool	ft::isalpha(int c)
{
	if (c >= 'A' && c <= 'Z')
		return (true);
	if (c >= 'a' && c <= 'z')
		return (true);
	return (false);
}

bool	ft::isdigit(int c)
{
	return ((c >= '0' && c <= '9') ? true : false);
}

bool	ft::isdigit(const std::string &s)
{
	std::string::const_iterator	first = s.begin();
	std::string::const_iterator	last = s.end();

	while (first != last)
	{
		if (!ft::isdigit((int)*first))
			return (false);
		++first;
	}
	return (true);
}

static size_t			push_marker(std::vector<std::string>	*marker, std::string const &mask, size_t start, size_t ret, std::string mark)
{
	std::string		sub;

	if (start < ret)
	{
		sub = mask.substr(start, ret - start);
		marker->push_back(sub);
	}
	marker->push_back(mark);
	start = ret + 1;
	return (start);
}

static std::vector<std::string>		*parse_marker(std::string const &mask)
{
	std::vector<std::string>	*marker;
	size_t	start;
	size_t	s_ret;
	size_t	q_ret;

	start = 0;
	marker = new std::vector<std::string>;
	while (start < mask.length())
	{
		s_ret = mask.find('*', start);
		q_ret = mask.find('?', start);
		if (s_ret < q_ret)
			start = push_marker(marker, mask, start, s_ret, "*");
		else if (q_ret < s_ret)
			start = push_marker(marker, mask, start, q_ret, "?");
		else
		{
			if (start != mask.length())
				marker->push_back(mask.substr(start, mask.length() - start));
			break;
		}
	}
	return (marker);
}

static bool		check_str(std::string const &str, std::vector<std::string> &marker)
{
	size_t	start;
	size_t	ret;
	size_t	i = 0;
	std::vector<std::string>::iterator	iter;
	
	start = 0;
	while (i < marker.size())
	{
		if (marker[i] == "?")
		{
			i++;
			if (i == marker.size())
			{
				if (str.length() - start > 1)
					return (false);
				return (true);
			}
			if (marker[i] == "?" || marker[i] == "*")
			{
				i++;
				continue;
			}
			ret = str.find(marker[i], start); // ret: 새 위치 start: 이전 위치
			if (ret == std::string::npos || ret - start > 1)
				return (false);
			start = ret + (marker[i]).length();
			i++;
			if (i == marker.size() && start != str.size())
				return (false);
		}
		else if (marker[i] == "*")
		{
			i++;
		}
		else
		{
			ret = str.find(marker[i], start);
			if (ret == std::string::npos)
				return (false);
			start = ret + (marker[i]).length();
			if (i + 1 == marker.size())
			{
				if (i == 0 && (ret + 1 != (marker[i].length())))
					return (false);
				else if (start == str.length())
					return (true);
				else
					return (false);
			}
			i++;
		}
	}
	return (true);
}

bool	ft::check_mask(std::string const &str, std::string const &mask)
{
	bool							ret;
	std::vector<std::string>	*marker;

	if (str == mask)
		return (true);
	marker = parse_marker(mask);
	ret = check_str(str, *marker);
	delete marker;
	return (ret);
}

void	ft::get_current_time(std::string &result)
{
	time_t		t;
	std::string	*tmp;

	time(&t);
	result = ctime(&t);
	// "Www Mmm dd hh:mm:ss yyyy"
	ft::split(result, ' ', tmp);
	//요일 월 일 연도 -- 시:분 UTC
	tmp[3].resize(5);
	tmp[4].resize(4);
	result.clear();
	result += tmp[0] + " " + tmp[1] +  " " + tmp[2] + " " + tmp[4] + " -- " + tmp[3] + " UTC";
	// Monday April 05 2021 -- 04:25 UTC - 출력 값
	delete[] tmp;
}

void	ft::get_up_time(time_t start, std::string &result)
{
	time_t		t;
	std::string	*tmp;
	int			cnt;

	time(&t);
	t -= start;
	result = "Server Up ";
	result += ft::itos((long)(t / (60 * 60 * 24))) + " days ";
	t = t % (60 * 60 * 24);
	result += ft::itos((long)(t / (60 * 60))) + ":";
	t = t % (60 * 60);
	if ((t / 60) == 0)
		result += "0";
	result += ft::itos((long)(t / 60)) + ":";
	t = t % 60;
	if ((t / 10) == 0)
		result += "0";
	result += ft::itos(t);
	delete[] tmp;
}

static std::string		remember_to_buf(std::string &remember)
{
	std::string		result;
	
	for (int i = 0; i < remember.length(); i++)
	{
		if (remember.at(i) != ASCII_CONST::CR && remember.at(i) != ASCII_CONST::LF)
			result += remember.at(i);
		else
		{
			result += ASCII_CONST::LF;
			if (remember.at(i) == ASCII_CONST::CR &&
				(i + 1) < remember.length() && remember.at(i + 1) == ASCII_CONST::LF)
				i++;
			remember = remember.substr(i + 1);
			return (result);
		}
	}
	remember.clear();
	return (result);
}

static int		break_return(char *buf, char *buffer, int insert_idx, int *len, std::string remember)
{
	for (int i = 0; buf[i] != 0; i++)
	{
		buffer[i] = buf[i];
	}
	buffer[insert_idx] = 0;
	*len = insert_idx;
	if (remember.empty())
		return (0);
	else
		return (1);
}

int	ft::ssl_read_until_crlf(int fd, char *buffer, int *len, SSL *ssl)
{
	int					i = 0;
	int					read_size = 0;
	int					insert_idx = 0;
	char				buf[BUFFER_SIZE + 1];
	static std::string	remember[OPEN_MAX];
	int					rem_size = 0;

	memset(buf, 0, BUFFER_SIZE);
	if (!remember[fd].empty())
	{
		std::string	result = remember_to_buf(remember[fd]);
		rem_size = result.length();
		strncpy(buf, result.c_str(), rem_size);
		insert_idx = rem_size;
	}
	while (insert_idx < BUFFER_SIZE)
	{
		if (remember[fd].empty())
		{
			int	ret = 0;
			if ((read_size = SSL_read(ssl, buf + insert_idx, BUFFER_SIZE - insert_idx)) <= 0)
			{
				ret = SSL_get_error(ssl, read_size);
				if (ret == SSL_ERROR_WANT_READ)
				{
					rem_size = 0;
					remember[fd] += buf;
					return (2);
				}
				else
					return (break_return(buf, buffer, insert_idx, len, remember[fd]));
			}
			buf[insert_idx + read_size] = 0;
		}
		else
		{
			if (insert_idx >= 1 && buf[insert_idx - 1] != '\n')
			{
				std::string	result = remember_to_buf(remember[fd]);
				rem_size = result.length();
				strncpy(buf + insert_idx, result.c_str(), rem_size);
				insert_idx += rem_size;
			}
		}
		for (i = 0; i < read_size + rem_size; i++)
		{
			if (buf[i] == ASCII_CONST::CR || buf[i] == ASCII_CONST::LF)
			{
				if (rem_size == 0)
				{
					strncpy(buffer, buf, i + 1);
					buffer[i + insert_idx + 1] = 0;
					if (buffer[i] == '\r')
						buffer[i] = '\n';
				}
				else
				{
					strncpy(buffer, buf, i + 1);
					buffer[i + 1] = 0;
					if (buffer[i] == '\r')
						buffer[i] = '\n';
				}
				if (buf[i + 1] == ASCII_CONST::LF)
					++i;
				// strncpy(buffer + (rem_size == 0 ? insert_idx : 0), buf, i + 1);
				// buffer[i + (rem_size == 0 ? insert_idx : 0) + 1] = 0;
				for (int j = 1; buf[i + j]; ++j)
					remember[fd] += buf[i + j];
				*len = i + insert_idx;
				if (remember[fd].empty())
					return (0);
				return (1);
			}
		}
		rem_size = 0;
		remember[fd] += buf;
		return (2);
	}
	return (break_return(buf, buffer, insert_idx, len, remember[fd]));
}

int	ft::read_until_crlf(int fd, char *buffer, int *len)
{
	int					i = 0;
	int					read_size = 0;
	int					insert_idx = 0;
	char				buf[BUFFER_SIZE + 1];
	static std::string	remember[OPEN_MAX];
	int					rem_size = 0;

	memset(buf, 0, BUFFER_SIZE);
	if (!remember[fd].empty())
	{
		std::string	result = remember_to_buf(remember[fd]);
		rem_size = result.length();
		strncpy(buf, result.c_str(), rem_size);
		insert_idx = rem_size;
	}
	while (insert_idx < BUFFER_SIZE)
	{
		if (remember[fd].empty())
		{
			if ((read_size = read(fd, buf + insert_idx, BUFFER_SIZE - insert_idx)) == -1)
				break;
			else if (read_size == 0)
				break;
			buf[insert_idx + read_size] = 0;
		}
		else
		{
			if (insert_idx >= 1 && buf[insert_idx - 1] != '\n')
			{
				std::string	result = remember_to_buf(remember[fd]);
				rem_size = result.length();
				strncpy(buf + insert_idx, result.c_str(), rem_size);
				insert_idx += rem_size;
			}
		}
		for (i = 0; i < read_size + rem_size; i++)
		{
			if (buf[i] == ASCII_CONST::CR || buf[i] == ASCII_CONST::LF)
			{
				if (rem_size == 0)
				{
					strncpy(buffer, buf, i + 1);
					buffer[i + insert_idx + 1] = 0;
					if (buffer[i] == '\r')
						buffer[i] = '\n';
				}
				else
				{
					strncpy(buffer, buf, i + 1);
					buffer[i + 1] = 0;
					if (buffer[i] == '\r')
						buffer[i] = '\n';
				}
				if (buf[i + 1] == ASCII_CONST::LF)
					++i;
				for (int j = 1; buf[i + j]; ++j)
					remember[fd] += buf[i + j];
				*len = i + insert_idx;
				if (remember[fd].empty())
					return (0);
				return (1);
			}
		}
		rem_size = 0;
		remember[fd] += buf;
		return (2);
	}
	for (int i = 0; buf[i] != 0; i++)
	{
		buffer[i] = buf[i];
	}
	buffer[insert_idx] = 0;
	*len = insert_idx;
	if (remember[fd].empty())
		return (0);
	else
		return (1);
}

void	ft::ltrim(std::string & str, char c)
{
	while (*(str.begin()) == c)
		str.erase(str.begin());
}

void	ft::rtrim(std::string & str, char c)
{
	while (*(str.end() - 1) == c)
		str.erase(str.end() - 1);
}
