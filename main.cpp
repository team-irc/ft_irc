#include "ft_irc.hpp"
#include <signal.h>

int main(int argc, char **argv)
{
	try
	{
		IrcServer server(argc, argv);
		signal(SIGING, (void *)server.sigint_handler;
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