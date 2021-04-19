#include "SSL_Socket.hpp"

int create_socket(int port)
{
    int s;
    struct sockaddr_in addr;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        throw ("Unable to create socket\n");
    if (fcntl(s, F_SETFL, O_NONBLOCK) == -1)
		throw("Unable to fcntl NON_BLOCK\n");
    if (bind(s, (struct sockaddr*)&addr, sizeof(addr)) < 0)
        throw ("Unable to bind\n");
    if (listen(s, 1) < 0)
        throw ("Unable to listen\n");
    return s;
}

SSL_Socket::SSL_Socket(std::string const &port) : _ctx(0), _ssl(0), Socket(port.c_str())
{
	init_openssl();
	_ctx = create_context();
	configure_context(_ctx);
    this->bind();
    this->listen();
}

SSL_Socket::~SSL_Socket()
{
}

void		SSL_Socket::init_openssl()
{ 
    SSL_load_error_strings();	
    OpenSSL_add_ssl_algorithms();
}

void		SSL_Socket::cleanup_openssl()
{
    EVP_cleanup();
}

SSL_CTX 	*SSL_Socket::create_context()
{
    const SSL_METHOD *method;
    method = SSLv23_server_method();

    _ctx = SSL_CTX_new(method);
    if (!_ctx)
	{
		std::cerr << "Unable to create SSL context\n";
		ERR_print_errors_fp(stderr);
		exit(EXIT_FAILURE);
    }

    return _ctx;
}

void		SSL_Socket::configure_context(SSL_CTX *ctx)
{
    SSL_CTX_set_ecdh_auto(ctx, 1);

    /* Set the key and cert */
    if (SSL_CTX_use_certificate_file(ctx, "cert.pem", SSL_FILETYPE_PEM) <= 0)
	{
        ERR_print_errors_fp(stderr);
		exit(EXIT_FAILURE);
    }

    if (SSL_CTX_use_PrivateKey_file(ctx, "key.pem", SSL_FILETYPE_PEM) <= 0 )
	{
        ERR_print_errors_fp(stderr);
		exit(EXIT_FAILURE);
    }
}