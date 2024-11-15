ft_irc
Welcome to ft_irc, a project that reproduces the functioning of an IRC (Internet Relay Chat) server using C++98. 

🔮 Definition
IRC (Internet Relay Chat) is a protocol for real-time text messaging between internet-connected computers, created in 1988. It is mainly used for group discussion in chat rooms called “channels,” though it also supports private messaging, data transfer, and a variety of server-side and client-side commands.

💾 Subject Requirements
Code the IRC server in C++98.
The binary should be executable as follows: ./ircserv <port> <password>.
Use the poll() function only once in the entire project.
Communication between client and server should use TCP/IP (v4 or v6).
Mandatory Features:
Registration system (nickname, username).
Ability to join channels, send and receive private messages.
Messages sent in a channel must be received by all clients in that channel.
Implement user roles: normal users and operators, with operator-specific commands.
Handle signals (CTRL+D, CTRL+Z, SIGINT) and ensure no memory leaks.
Bonus Features:
Implement a bot and file transfer.
🔆 Commands Implemented
Command	Description
INVITE	Invites a user to a channel.
JOIN	Joins a channel with an optional key.
KICK	Removes a user from a channel.
KILL	Terminates the connection between a client and server (Operator-only command).
LIST	Lists all channels with basic information.
MODE	Sets or removes options/modes for users or channels.
MOTD	Retrieves the "Message of the Day" from the server.
NAMES	Lists nicknames in a channel.
NICK	Sets or changes a client's nickname.
NOTICE	Sends notices between users or to a channel.
OPER	Grants IRC operator privileges to a user.
PART	Leaves a channel.
PASS	Sets a connection password before registration.
PING	Checks if the connection is still active.
PRIVMSG	Sends private messages between users or to channels.
QUIT	Terminates the client’s connection to the server.
TOPIC	Views or changes a channel's topic.
USER	Specifies the username and real name during registration.
🎁 Tips, Graphs, and Resources
📚 Helpful Links
Chirc: Learn about IRC and get started with your project.
Beej's Guide to Network Programming: A must-read for understanding sockets, poll() function, system calls, and their order.
Modern IRC Client Protocol: Detailed command descriptions, correct syntax, and expected numerical replies.
📫 Tips
Project Roadmap:

Establish a working client/server socket connection.
Implement signal handling (CTRL+C, CTRL+D, CTRL+Z).
Develop the client registration system (NICK, USER, PASS).
Build server commands (PING, OPER, KILL).
Implement channel operation commands (JOIN, PART, INVITE, KICK, PRIVMSG, NOTICE).
Common Issues:

If your code works with irssi but not with nc, remember that nc requires message concatenation until CRLF is detected.
Each client should have a separate read and send buffer to manage signals and ensure efficient communication.
Formatting Replies:

Join a live IRC server (e.g., DALNET), enable raw logging, and review the client/server interactions for the correct formatting of replies.
🛠️ Useful Code Macros
cpp
Copy code
#define user_id(nickname, username) (":" + nickname + "!" + username + "@localhost")
#define RPL_INVITE(user_id, invited, channel) (user_id + " INVITE " + invited + " #" + channel + "\r\n")
#define ERR_INVALIDMODEPARAM(client, channel, mode, password) ("696 " + client + " #" + channel + " " + mode + " " + password + " : password must only contain alphabetic characters\r\n")
📖 About the Project
This project explores network programming, focusing on creating a functional IRC server in C++. It covers key concepts such as socket programming, connection handling, and data exchange, offering a practical understanding of building network-based applications.

