#include "SocketSet.hpp"

SocketSet::SocketSet()
{
	FD_ZERO(&_read);
}

SocketSet::SocketSet(SocketSet const &ref) :
	_vec(ref._vec), _read(ref._read)
{}

SocketSet	&SocketSet::operator= (SocketSet const &ref)
{
	_vec = ref._vec;
	_read = ref._read;
	return (*this);
}

SocketSet::~SocketSet()
{
	std::vector<Socket *>::iterator begin = _vec.begin();
	std::vector<Socket *>::iterator end = _vec.end();

	while (begin != end)
	{
		FD_CLR((*begin)->get_fd(), &_read);
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
	FD_SET(new_sock->get_fd(), &_read);
	return (new_sock->get_fd());
}

// UNKNOWN -> CLIENT, UNKNOWN -> SERVER 변경 시 호출 필요
void		SocketSet::change_socket_type(int fd, SOCKET_TYPE type)
{
	std::vector<Socket *>::iterator begin = _vec.begin();
	std::vector<Socket *>::iterator end = _vec.end();

	while (begin != end)
	{
		if ((*begin)->get_fd() == fd)
		{
			(*begin)->set_type(type);
			return ;
		}
		begin++;
	}
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
			FD_CLR((*begin)->get_fd(), &_read);
			_vec.erase(begin);
			return ;
		}
		begin++;
	}
}

fd_set		&SocketSet::get_read_fds()
{ return (_read); }

fd_set const	&SocketSet::get_read_fds() const
{ return (_read); }

std::vector<Socket *>	SocketSet::get_connect_sockets()
{ return (_vec); }

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