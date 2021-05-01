#include "IrcServer.hpp"
		
std::string Reply::_user_name = "";
std::string Reply::_server_name = "";

static void sigint_handler(int _)
{
	(void)_;
	std::cout << "\b \b\b \bSIGINT" << std::endl;
	exit(0);
}

int main(int argc, char **argv)
{
	signal(SIGINT, sigint_handler);
	try
	{
		IrcServer server(argc, argv);
		server.run(argc);
	}
	catch(Error &e)
	{
		std::cerr << e.what() << '\n';
	}
}