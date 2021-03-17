#include <iostream>
#include <string.h>

int		main()
{
	std::string		str;

	str = std::string();
	std::cout << str << std::endl;
	if (str == "")
		std::cout << "true\n";
	return (0);
}