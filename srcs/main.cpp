#include "ft_irc.hpp"
#include <signal.h>

std::string Reply::_user_name = "";
std::string Reply::_server_name = "";

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
	catch(char const*err)
	{
		std::cerr << err << '\n';
	}
}