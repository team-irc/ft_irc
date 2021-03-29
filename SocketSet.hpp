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
		void					change_socket_type(int fd, SOCKET_TYPE type);
		fd_set					&get_read_fds();
		fd_set const			&get_read_fds() const;
		std::vector<Socket *>	get_connect_sockets();
		void					show_info();
	private:
		std::vector<Socket *>	_vec;
		fd_set					_read;
};

#endif