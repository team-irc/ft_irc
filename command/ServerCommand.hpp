#ifndef SERVER_COMMAND_HPP
# define SERVER_COMMAND_HPP

# include "Command.hpp"
# include <map>

/*
	Command: SERVER
Parameters: <servername> <hopcount> <info>

The server message is used to tell a server that the other end of a
new connection is a server. This message is also used to pass server
data over whole net.  When a new server is connected to net,
information about it be broadcast to the whole network.  <hopcount>
is used to give all servers some internal information on how far away
all servers are.  With a full server list, it would be possible to
construct a map of the entire server tree, but hostmasks prevent this
from being done.

The SERVER message must only be accepted from either (a) a connection
which is yet to be registered and is attempting to register as a
server, or (b) an existing connection to another server, in  which
case the SERVER message is introducing a new server behind that
server.

Most errors that occur with the receipt of a SERVER command result in
the connection being terminated by the destination host (target
SERVER).  Error replies are usually sent using the "ERROR" command
rather than the numeric since the ERROR command has several useful
properties which make it useful here.

If a SERVER message is parsed and attempts to introduce a server
which is already known to the receiving server, the connection from
which that message must be closed (following the correct procedures),
since a duplicate route to a server has formed and the acyclic nature
of the IRC tree broken.

Numeric Replies:

		ERR_ALREADYREGISTRED
*/

class ServerCommand : public Command
{
private:

public:
	ServerCommand();
	virtual ~ServerCommand();
	ServerCommand(ServerCommand const &copy);
	ServerCommand	&operator=(ServerCommand const &ref);
	void	run(IrcServer &irc);
private:
	bool deal_exception(IrcServer &irc);
};

#endif