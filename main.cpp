#include "ft_irc.hpp"

int main(int argc, char **argv)
{
	try
	{
		IrcServer server(argc, argv);
		server.run(argc);
	}
	catch(Error &e)
	{
		std::cerr << e.what() << '\n';
	}
	catch(char *err)
	{
		std::cerr << err << '\n';
	}
}