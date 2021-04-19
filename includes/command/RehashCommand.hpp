#ifndef REHASH_COMMAND_HPP
# define REHASH_COMMAND_HPP

# include "Command.hpp"

/*

5.2 Rehash message

      Command: REHASH
   Parameters: None

   The rehash message can be used by the operator to force the server to
   re-read and process its configuration file.

   Numeric Replies:

        RPL_REHASHING                   ERR_NOPRIVILEGES

Examples:

REHASH                          ; message from client with operator
                                status to server asking it to reread its
                                configuration file.

*/

class RehashCommand: public Command
{
public:
    RehashCommand();
    ~RehashCommand();
public:
    void	run(IrcServer &irc);
};

#endif