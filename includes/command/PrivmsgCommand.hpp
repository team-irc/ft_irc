#ifndef PRIVMSGCOMMAND_HPP
# define PRIVMSGCOMMAND_HPP

# include "Command.hpp"

/*
      Command: PRIVMSG
   Parameters: <receiver>{,<receiver>} <text to be sent>

   PRIVMSG is used to send private messages between users.  <receiver>
   is the nickname of the receiver of the message.  <receiver> can also
   be a list of names or channels separated with commas.

   The <receiver> parameter may also me a host mask  (#mask)  or  server
   mask  ($mask).   In  both cases the server will only send the PRIVMSG
   to those who have a server or host matching the mask.  The mask  must
   have at  least  1  (one)  "."  in it and no wildcards following the
   last ".".  This requirement exists to prevent people sending messages
   to  "#*"  or "$*",  which  would  broadcast  to  all  users; from
   experience, this is abused more than used responsibly and properly.
   Wildcards are  the  '*' and  '?'   characters.   This  extension  to
   the PRIVMSG command is only available to Operators.

   Numeric Replies:

	   ERR_NORECIPIENT                 ERR_NOTEXTTOSEND
	   ERR_CANNOTSENDTOCHAN            ERR_NOTOPLEVEL
	   ERR_WILDTOPLEVEL                ERR_TOOMANYTARGETS
	   ERR_NOSUCHNICK
	   RPL_AWAY

   Examples:

:Angel PRIVMSG Wiz :Hello are you receiving this message ?
				; Message from Angel to Wiz.

PRIVMSG Angel :yes I'm receiving it !receiving it !'u>(768u+1n) .br ;
				Message to Angel.

PRIVMSG jto@tolsun.oulu.fi :Hello !
				; Message to a client on server
*/

class Channel;
class Member;

class PrivmsgCommand : public Command
{
public:
	PrivmsgCommand();
	~PrivmsgCommand();
public:
	void	run(IrcServer &irc);
private:
	void	add_prefix(IrcServer &irc);
	void	check_receiver(IrcServer &irc, const std::string &recv,
		std::vector<std::string> &send_nick, std::vector<int> &send_channel_fd);
	void	send_channel(IrcServer &irc, Channel &channel, Member *sender,
		std::vector<int> &send_channel_fd);
	void	send_member(IrcServer &irc, Member &member,
		std::vector<std::string> &send_nick);
};

#endif