/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketSet.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hna <hna@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/24 22:17:58 by marvin            #+#    #+#             */
/*   Updated: 2021/02/25 00:59:37 by hna              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_SET_HPP
# define SOCKET_SET_HPP

# include "Socket.hpp"
# include <sys/select.h>
# include <vector>

class	SocketSet
{
	public:
		SocketSet();
		SocketSet(SocketSet const &ref);
		SocketSet				&operator=(SocketSet const &ref);
		virtual ~SocketSet();
		void					add_socket(Socket *new_sock);
		void					remove_socket(Socket *del);
		fd_set					get_server_fds() const
		fd_set					get_client_fds() const
	private:
		std::vector<Socket *>	_vec;
		fd_set					_server_sock;
		fd_set					_client_sock;
};

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

// 스택 영역에 생성한거 주소값 넣으면 안됨
void		SocketSet::add_socket(Socket *new_sock)
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
	if (new_sock->get_type() == SERVER)
		FD_SET(new_sock->get_fd(), &_server_sock);
	else
		FD_SET(new_sock->get_fd(), &_client_sock);
}

void		SocketSet::remove_socket(Socket *del)
{
	std::vector<Socket *>::iterator begin = _vec.begin();
	std::vector<Socket *>::iterator end = _vec.end();

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

fd_set		SocketSet::get_server_fds() const
{ return (_server_sock); }

fd_set		SocketSet::get_client_fds() const
{ return (_client_sock); }

#endif