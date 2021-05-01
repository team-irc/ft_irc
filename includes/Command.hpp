#ifndef COMMAND_HPP
# define COMMAND_HPP

# include "SocketSet.hpp"
# include "Message.hpp"
# include "Reply.hpp"
# include "utils.hpp"

/*
<message>  ::= [':' <prefix> <SPACE> ] <command> <params> <crlf>
<prefix>   ::= <servername> | <nick> [ '!' <user> ] [ '@' <host> ]
<command>  ::= <letter> { <letter> } | <number> <number> <number>
<SPACE>    ::= ' ' { ' ' }
<params>   ::= <SPACE> [ ':' <trailing> | <middle> <params> ]

<middle>   ::= <Any *non-empty* sequence of octets not including SPACE
               or NUL or CR or LF, the first of which may not be ':'>
<trailing> ::= <Any, possibly *empty*, sequence of octets not including
                 NUL or CR or LF>

<crlf>     ::= CR LF
<target>     ::= <to> [ "," <target> ]
<to>         ::= <channel> | <user> '@' <servername> | <nick> | <mask>
<channel>    ::= ('#' | '&') <chstring>
<servername> ::= <host>
<host>       ::= see RFC 952 [DNS:4] for details on allowed hostnames
<nick>       ::= <letter> { <letter> | <number> | <special> }
<mask>       ::= ('#' | '$') <chstring>
<chstring>   ::= <any 8bit code except SPACE, BELL, NUL, CR, LF and
					comma (',')>

Other parameter syntaxes are:

<user>       ::= <nonwhite> { <nonwhite> }
<letter>     ::= 'a' ... 'z' | 'A' ... 'Z'
<number>     ::= '0' ... '9'
<special>    ::= '-' | '[' | ']' | '\' | '`' | '^' | '{' | '}'
<nonwhite>   ::= <any 8bit code except SPACE (0x20), NUL (0x0), CR
                     (0xd), and LF (0xa)>
*/

class IrcServer;

class Command
{
protected:
	Message		_msg;
	size_t		_call_count;
public:
	Command();
	Command(const Command &ref);
	Command &operator=(const Command &ref);
	void	set_message(const Message &msg);
	virtual ~Command();
public:
	void execute(IrcServer &irc);
	virtual void run(IrcServer &irc) = 0;
	size_t		get_count();

private:
	void		set_reply_header(IrcServer &irc);
};

# include "IrcServer.hpp"

#endif