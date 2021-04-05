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

char	*get_up_time(char *start_time)
{
	time_t		t;

	time(&t);
	return (ctime(&t));
}

int	ft::read_until_crlf(int fd, char *buffer, int *len)
{
	if (DEBUG)
		std::cout << "read_until_crlf start\n";
	int					i = 0;
	int					read_size = 0;
	int					insert_idx = 0;
	char				buf[BUFFER_SIZE];
	static std::string	remember;
	int					rem_size = 0;

	memset(buf, 0, BUFFER_SIZE);
	// buf에 remember를 삽입
	if (!remember.empty())
	{
		rem_size = remember.length();
		strncpy(buf, remember.c_str(), rem_size);
		insert_idx += rem_size;
	}
	while (insert_idx < BUFFER_SIZE)
	{
		if (remember.empty())
		{
			if (!(read_size = read(fd, buf, BUFFER_SIZE - insert_idx)))
				break;
		}
		else
		{
			strncpy(buf, remember.c_str(), rem_size);
			remember.clear();
		}
		for (i = 0; i < read_size + rem_size; i++)
		{
			if (buf[i] == ASCII_CONST::CR || buf[i] == ASCII_CONST::LF)
			{
				if (rem_size == 0)
				{
					strncpy(buffer + insert_idx, buf, i + 1);
					buffer[i + insert_idx + 1] = 0;
				}
				else
				{
					strncpy(buffer, buf, i + 1);
					buffer[i + 1] = 0;
				}
				// strncpy(buffer + (rem_size == 0 ? insert_idx : 0), buf, i + 1);
				// buffer[i + (rem_size == 0 ? insert_idx : 0) + 1] = 0;
				for (int j = 1; buf[i + j]; ++j)
					remember += buf[i + j];
				*len = i + insert_idx;
				if (remember.empty())
					return (0);
				return (1);
			}
		}
		rem_size = 0;
		// write(1, buf, read_size);
		strncpy(buffer + insert_idx, buf, read_size);
		insert_idx += read_size;
	}
	buffer[insert_idx] = 0;
	*len = BUFFER_SIZE;
	return (0);
}