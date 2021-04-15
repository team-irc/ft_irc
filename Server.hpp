#ifndef SERVER_HPP
# define SERVER_HPP

# include <string>
# include "Socket.hpp"

class Server
{
private:
	int				_hopcount;
	std::string		_info;
	std::string		_name;
	std::string		_host;
	std::string		_password;
	int				_token;
	Socket			*_connected_socket;

public:
	Server();
	Server(const std::string &server_name, const std::string &hopcount, int token, const std::string &info);
	~Server();

	int		get_hopcount();
	void	set_hopcount(int hopcount);
	
	std::string		get_info();
	void			set_info(std::string const &info);

	std::string		get_name();
	void			set_name(std::string const &name);

	std::string		get_password();
	void			set_password(std::string const &password);

	int				get_token();
	void			set_token(int token);

	Socket		 	*get_socket();
	void			set_socket(Socket *socket);

private:
	Server(Server const &copy);
	Server &operator=(Server const &ref);
};



#endif