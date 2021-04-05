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

static size_t						push_marker(std::vector<std::string>	*marker, std::string const &mask, size_t start, size_t ret, std::string mark)
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

static bool	check_str(std::string const &str, std::vector<std::string> &marker)
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
				if (start == str.length())
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

	marker = parse_marker(mask);
	ret = check_str(str, *marker);
	delete marker;
	return (ret);
}