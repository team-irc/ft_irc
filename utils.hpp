#ifndef UTILS_HPP
# define UTILS_HPP

#include <iostream>

namespace ft
{
	void error_handling(const std::string buf)
	{
		std::cerr << buf << std::endl;
		exit(1);
	}

	int atoi(const char * nptr)
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
	};

	std::string * split(const std::string str, char c)
	{
		int size;
		int str_counter;
		std::string * ret;

		size = 0;
		str_counter = 0;
		for (int i = 0; i < str.length(); ++i)
		{
			if (str[i] == c)
				++size;
		}
		++size;
		ret = new std::string[size];
		for (int i = 0; i < size; ++i)
		{
			for (; (str[str_counter] != c && str_counter < str.length()); ++str_counter)
				ret[i] += str[str_counter];
			while (str[str_counter] == c)
				++str_counter;
		}
		return (ret);
	};
};

#endif