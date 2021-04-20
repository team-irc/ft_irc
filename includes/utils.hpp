#ifndef UTILS_HPP
# define UTILS_HPP

# include <iostream>
# include <time.h>
# include <limits.h>
# include <string>
# ifdef __linux__
# include <string.h>
# endif
# include <vector>
# include <ctime>
# include <unistd.h>
# include <openssl/ssl.h>
# include <openssl/err.h>

# define BUFFER_SIZE 512
# ifdef __linux__
# define OPEN_MAX 1024
# endif

namespace ft
{

	int			atoi(const char * nptr);

	std::string itos(int n);
	std::string itos(long n);
	std::string itos(unsigned short n);
	std::string itos(unsigned long n);

	int			split(const std::string str, char c, std::string *& ret);

	char		*strchr(const char *s, int c);

	bool		isalpha(int c);

	bool		isdigit(int c);
	bool		isdigit(const std::string &s);

	bool		check_mask(std::string const &str, std::string const &mask);

	void		get_current_time(std::string &result);

	void		get_up_time(time_t start, std::string &result);

	int			read_until_crlf(int fd, char *buffer, int *len);
	int			ssl_read_until_crlf(int fd, char *buffer, int *len, SSL *ssl);

	void		ltrim(std::string & str, char c);

	void		rtrim(std::string & str, char c);

};

namespace	ASCII_CONST
{
	const char		CR = 13;
	const char		LF = 10;
};

#endif