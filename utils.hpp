#ifndef UTILS_HPP
# define UTILS_HPP

#include <iostream>
#include <string>
#include <vector>

namespace ft
{

	int		atoi(const char * nptr);

	int		split(const std::string str, char c, std::string *& ret);

	char	*strchr(const char *s, int c);

	bool	isalpha(int c);

	bool	isdigit(int c);

	bool	check_mask(std::string const &str, std::string const &mask);

};

namespace	ASCII_CONST
{
	const char		CR = 13;
	const char		LF = 10;
};

#endif