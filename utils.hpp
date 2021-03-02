#ifndef UTILS_HPP
# define UTILS_HPP

#include <iostream>

namespace ft
{

	int atoi(const char * nptr);

	int split(const std::string str, char c, std::string *& ret);

};

#endif