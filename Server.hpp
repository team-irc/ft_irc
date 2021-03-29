#ifndef SERVER_HPP
# define SERVER_HPP

class Server
{
private:
	int				_fd;
	std::string		_name;
	std::string		_password;

public:
	Server();
	~Server();
};



#endif