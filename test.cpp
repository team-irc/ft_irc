#include <iostream>
#include <string>

int		main()
{
	char	buf[10] = "abcd";
	std::cout << buf << std::endl;

	char	buf2[10];
	strncpy(buf2, buf, 1);
	std::cout << buf2 <<std::endl;
	return (0);
}