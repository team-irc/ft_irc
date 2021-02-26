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
		int						add_socket(Socket *new_sock);
		Socket					*find_socket(int fd);
		void					remove_socket(Socket *del);
		fd_set					&get_server_fds();
		fd_set const			&get_server_fds() const;
		fd_set					&get_client_fds();
		fd_set const			&get_client_fds() const;
		fd_set					&get_listen_fds();
		fd_set const			&get_listen_fds() const;
		void					show_info();
	private:
		std::vector<Socket *>	_vec;
		fd_set					_server_sock;
		fd_set					_client_sock;
		fd_set					_listen_sock;
};

#endif