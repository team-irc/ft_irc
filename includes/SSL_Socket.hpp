#ifndef SSL_SOCKET_HPP
# define SSL_SOCKET_HPP

# include "Socket.hpp"
# include <fcntl.h>
# include <openssl/ssl.h>
# include <openssl/err.h>

class SSL_Socket : public Socket
{
private:
	SSL_CTX		*_ctx;
    SSL			*_ssl;

public:
	SSL_Socket(std::string const &port, SSL_CTX *ctx);
	SSL_Socket(SSL_CTX *ctx);
	~SSL_Socket();
	
	SSL_Socket			*accept(SSL_CTX *ctx);
	static SSL_Socket	*connect(const char *connect_srv, SSL_CTX *ctx);
	int					read(int fd, char *buffer, int *len);
	void				write(char const *msg);
	void				write(Reply rpl);
	void				init_openssl();
	void				cleanup_openssl();
	SSL_CTX				*create_context();
	void				configure_context(SSL_CTX *ctx);
};

#endif
