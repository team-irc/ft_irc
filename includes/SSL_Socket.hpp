#ifndef SSL_SOCKET_HPP
# define SSL_SOCKET_HPP

# include <openssl/ssl.h>
# include <openssl/err.h>
# include "Socket.hpp"

class SSL_Socket : public Socket
{
private:
	SSL_CTX		_ctx;
	SSL			_ssl;

public:
	SSL_Socket(/* args */);
	virtual ~SSL_Socket();

	int			init_openssl();
	SSL_CTX		*create_context();
	int			config_context(SSL_CTX *ctx);


};

SSL_Socket::SSL_Socket() : Socket()
{

}

SSL_Socket::~SSL_Socket()
{
}

int			SSL_Socket::init_openssl()
{
	SSL_load_error_strings();	
    OpenSSL_add_ssl_algorithms();
}

SSL_CTX		SSL_Socket::*create_context()
{
	const SSL_METHOD *method;
    SSL_CTX *ctx;

    method = SSLv23_server_method();

    ctx = SSL_CTX_new(method);
    if (!ctx) {
		perror("Unable to create SSL context");
		ERR_print_errors_fp(stderr);
		exit(EXIT_FAILURE);
    }

    return ctx;
}

int			SSL_Socket::config_context(SSL_CTX *ctx)
{
	 SSL_CTX_set_ecdh_auto(ctx, 1);

    /* Set the key and cert */
    if (SSL_CTX_use_certificate_file(ctx, "cert.pem", SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
	exit(EXIT_FAILURE);
    }

    if (SSL_CTX_use_PrivateKey_file(ctx, "key.pem", SSL_FILETYPE_PEM) <= 0 ) {
        ERR_print_errors_fp(stderr);
	exit(EXIT_FAILURE);
    }
}


#endif