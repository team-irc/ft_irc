#ifndef SSL_SOCKET_HPP
# define SSL_SOCKET_HPP

# include "Socket.hpp"
# include <openssl/ssl.h>
# include <openssl/err.h>

class SSL_Socket : public Socket
{
private:
	SSL_CTX		*_ctx;
    SSL			*_ssl;

public:
	SSL_Socket(std::string const &port);
	~SSL_Socket();
	
	void		init_openssl();
	void		cleanup_openssl();
	SSL_CTX		*create_context();
	void		configure_context(SSL_CTX *ctx);
};

#endif
