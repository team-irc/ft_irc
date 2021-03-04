#include "SocketSet.hpp"
#include "Error.hpp"

SocketSet::SocketSet()
{
	FD_ZERO(&_server_sock);
	FD_ZERO(&_client_sock);
}

SocketSet::SocketSet(SocketSet const &ref) :
	_vec(ref._vec), _server_sock(ref._server_sock), _client_sock(ref._client_sock)
{}

SocketSet	&SocketSet::operator= (SocketSet const &ref)
{
	_vec = ref._vec;
	_server_sock = ref._server_sock;
	_client_sock = ref._client_sock;
	return (*this);
}

SocketSet::~SocketSet()
{
	std::vector<Socket *>::iterator begin = _vec.begin();
	std::vector<Socket *>::iterator end = _vec.end();

	while (begin != end)
	{
		delete (*begin);
		begin++;
	}
}

int			SocketSet::add_socket(Socket *new_sock)
{
	std::vector<Socket *>::iterator begin = _vec.begin();
	std::vector<Socket *>::iterator end = _vec.end();

	while (begin != end)
	{
		if ((*begin)->get_fd() == new_sock->get_fd())
			throw (Error("same fd is already in use"));
		begin++;
	}
	_vec.push_back(new_sock);
	if (new_sock->get_type() == SockType::SERVER) {
		FD_SET(new_sock->get_fd(), &_server_sock);
		if (FD_ISSET(new_sock->get_fd(), &_server_sock))
			std::cout << "server socket " << new_sock->get_fd() << " add" << std::endl;
	}
	else if (new_sock->get_type() == SockType::CLIENT) {
		FD_SET(new_sock->get_fd(), &_client_sock);
		std::cout << "client socket add" << std::endl;
	}
	else {
		FD_SET(new_sock->get_fd(), &_listen_sock);
		std::cout << "listen socket add" << std::endl;
	}
	return (new_sock->get_fd());
}

Socket		*SocketSet::find_socket(int fd)
{
	std::vector<Socket *>::iterator begin = _vec.begin();
	std::vector<Socket *>::iterator end = _vec.end();

	while (begin != end)
	{
		if ((*begin)->get_fd() == fd)
			return (*begin);
		begin++;
	}
	return (NULL);
}

void		SocketSet::remove_socket(Socket *del)
{
	std::vector<Socket *>::iterator begin = _vec.begin();
	std::vector<Socket *>::iterator end = _vec.end();

	if (del == NULL)
		return ;
	while (begin != end)
	{
		if ((*begin)->get_fd() == del->get_fd())
		{
			_vec.erase(begin);
			return ;
		}
		begin++;
	}
}

fd_set		&SocketSet::get_server_fds()
{ return (_server_sock); }

fd_set const	&SocketSet::get_server_fds() const
{ return (_server_sock); }

fd_set		&SocketSet::get_client_fds()
{ return (_client_sock); }

fd_set const	&SocketSet::get_client_fds() const
{ return (_client_sock); }

fd_set			&SocketSet::get_listen_fds()
{ return (_listen_sock); }

fd_set const	&SocketSet::get_listen_fds() const
{ return (_listen_sock); }

void			SocketSet::show_info()
{
	std::vector<Socket *>::iterator begin = _vec.begin();
	std::vector<Socket *>::iterator end = _vec.end();

	while (begin != end)
	{
		(*begin)->show_info();
		begin++;
	}


}