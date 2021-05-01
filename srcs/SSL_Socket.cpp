#include "SSL_Socket.hpp"
#include "Reply.hpp"
#include "utils.hpp"

SSL_Socket::SSL_Socket(std::string const &port, SSL_CTX *ctx) : Socket(port.c_str())
{
    bind();
    listen();
	_ssl = SSL_new(ctx);
    SSL_set_fd(_ssl, _fd);
	fcntl(_fd, F_SETFL, O_NONBLOCK);
}

SSL_Socket::SSL_Socket(SSL_CTX *ctx) : Socket()
{
	_ssl = SSL_new(ctx);
}

SSL_Socket::~SSL_Socket()
{
	SSL_shutdown(_ssl);
	SSL_free(_ssl);
}

SSL_Socket  *SSL_Socket::accept(SSL_CTX *ctx)
{
	int						fd;
	SSL_Socket				*accepted_socket;
	struct sockaddr_in		accepted_addr;
	socklen_t				addr_size;
	int						set = 1;

	fd = ::accept(_fd, (struct sockaddr *)&accepted_addr, &addr_size);
	if (fd <= 0)
		throw (Error("SSL Socket accept error"));
	accepted_socket = new SSL_Socket(ctx);
	setsockopt(_fd, SOL_SOCKET, SO_NOSIGPIPE, (void *)&set, sizeof(int));
	accepted_socket->_fd = fd;
	memcpy(&(accepted_socket->_addr), &accepted_addr, addr_size);
	SSL_set_fd(accepted_socket->_ssl, accepted_socket->_fd);
	int ret = SSL_accept(accepted_socket->_ssl);
	// int error = ret;
	if (ret < 0)
	{
		if (SSL_get_error(_ssl, ret) != SSL_ERROR_WANT_ACCEPT &&
			SSL_get_error(_ssl, ret) != SSL_ERROR_SYSCALL)
		{
			delete (accepted_socket);
			return (NULL);
		}
	}

	std::cout << "SSL accepted\n";
	fcntl(accepted_socket->_fd, F_SETFL, O_NONBLOCK);
	return (accepted_socket);
}

SSL_Socket	*SSL_Socket::connect(const char *connect_srv, SSL_CTX *ctx)
{
	SSL_Socket				*connect_socket;
	struct sockaddr_in		connect_addr;
	std::pair<struct sockaddr_in, std::string>	pair;
	int						set = 1;

	memset(&connect_addr, 0, sizeof(connect_addr));
	pair = parsing_host_info(connect_srv);
	connect_addr = pair.first;

	connect_socket = new SSL_Socket(ctx);
	connect_socket->_fd = socket(AF_INET, SOCK_STREAM, 0);
	connect_socket->_addr = connect_addr;
	connect_socket->set_pass(pair.second);
	setsockopt(connect_socket->_fd, SOL_SOCKET, SO_NOSIGPIPE, (void *)&set, sizeof(int));
    SSL_set_fd(connect_socket->_ssl, connect_socket->_fd);
	if (connect_socket->_fd == -1)
		throw(Error("connect socket create error"));
	if (::connect(connect_socket->_fd, (struct sockaddr *)&connect_socket->_addr, sizeof(connect_socket->_addr)) == -1)
	{
		if (errno != EINPROGRESS)
			throw (Error("socket connect error"));
	}
	if (SSL_connect(connect_socket->_ssl) <= 0)
	{
		if (errno != EINPROGRESS)
			throw (Error("SSL_connect Error\n"));
	}
	fcntl(connect_socket->_fd, F_SETFL, O_NONBLOCK);
	return (connect_socket);
}

int			SSL_Socket::read(int fd, char *buffer, int *len)
{
	int		ret;

	ret = ft::ssl_read_until_crlf(fd, buffer, len, _ssl);
	std::cout << "[SSL_RECV] " << buffer << " [" << fd << "] " << "[" << show_type() << "]\n";
	if (*len > 0)
	{
		_recv_bytes += static_cast<size_t>(*len);
		_recv_cnt++;
	}
	return (ret);
}

void		SSL_Socket::write(char const *msg)
{
	std::cout << "[SSL_SEND] " << msg << " [" << _fd << "] "
			  << "[" << show_type() << "]\n";
	_sent_bytes += strlen(msg);
	_sent_cnt++;
	SSL_write(_ssl, msg, strlen(msg));
}

void		SSL_Socket::write(Reply rpl)
{
	std::cout << "[SSL_SEND] " << rpl.get_msg().c_str() << " [" << _fd << "] "
			  << "[" << show_type() << "]\n";
	_sent_bytes += strlen(rpl.get_msg().c_str());
	_sent_cnt++;
	SSL_write(_ssl, rpl.get_msg().c_str(), strlen(rpl.get_msg().c_str()));
}

void		SSL_Socket::init_openssl()
{ 
	SSL_load_error_strings();	
	OpenSSL_add_ssl_algorithms();
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
    if (SSL_CTX_use_certificate_file(ctx, "./ssl/cert.pem", SSL_FILETYPE_PEM) <= 0)
	{
		ERR_print_errors_fp(stderr);
		exit(EXIT_FAILURE);
	}

    if (SSL_CTX_use_PrivateKey_file(ctx, "./ssl/private.key", SSL_FILETYPE_PEM) <= 0 )
	{
		ERR_print_errors_fp(stderr);
		exit(EXIT_FAILURE);
	}
}
