#ifndef UTILS_HPP
# define UTILS_HPP

#include <iostream>
#include <ctime>

namespace ft
{

	int		atoi(const char * nptr);

	int		split(const std::string str, char c, std::string *& ret);

	char	*strchr(const char *s, int c);

	bool	isalpha(int c);

	bool	isdigit(int c);

	void	get_current_time(std::string &result);

	void	get_up_time(time_t start, std::string &result);

};

namespace	ASCII_CONST
{
	const char		CR = 13;
	const char		LF = 10;
};

#endif