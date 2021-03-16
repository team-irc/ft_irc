#ifndef UTILS_HPP
# define UTILS_HPP

#include <iostream>
#include <vector>

namespace ft
{

	int atoi(const char * nptr);

	int split(const std::string str, char c, std::string *& ret);

	class multiDimensionArray;

};

namespace	ASCII_CONST
{
	const char		CR = 13;
	const char		LF = 10;
};

#endif