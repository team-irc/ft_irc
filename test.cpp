#include <iostream>
#include <string>

int		main(int argc, char **argv)
{
	std::string		msg = "SERVER ";

	msg += argv[1];
	msg += " ";
	msg += std::to_string(0);
	std::cout << msg << std::endl;
	return (0);
}