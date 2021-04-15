#include "Server.hpp"

Server::Server()
{
}

Server::Server(const std::string &server_name, const std::string &hopcount, int token, const std::string &info) : _name(server_name), _token(token), _info(info)
{
	_hopcount = ft::atoi(hopcount.c_str());
}

Server::~Server()
{
}

Server::Server(Server const &copy) : _hopcount(copy._hopcount), _token(copy._token), _info(copy._info), _name(copy._name), _password(copy._password), _connected_socket(copy._connected_socket)
{
}

Server		&Server::operator=(Server const &ref)
{
	_hopcount = ref._hopcount;
	_token = ref._token;
	_info = ref._info;
	_name = ref._name;
	_password = ref._password;
	_connected_socket = ref._connected_socket;
	return (*this);
}

int		Server::get_hopcount() {return (_hopcount);}
void	Server::set_hopcount(int hopcount) {_hopcount = hopcount;}

std::string		Server::get_info() { return (_info); }
void			Server::set_info(std::string const &info) { _info = info; }

std::string		Server::get_name() { return (_name); }
void			Server::set_name(std::string const &name) { _name = name; }

std::string		Server::get_password() { return (_password); }
void			Server::set_password(std::string const &password) { _password = password; }

int				Server::get_token() { return (_token); }
void			Server::set_token(int token) { _token = token; }

Socket		 	*Server::get_socket() { return (_connected_socket); }
void			Server::set_socket(Socket *socket) { _connected_socket = socket; }