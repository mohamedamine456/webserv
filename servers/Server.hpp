#ifndef SERVER_HPP
#define SERVER_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>
#include <iostream>
#include <fcntl.h>

class Server {
	private:
		int     			serverFd;
		struct sockaddr_in	serverAddress;
	public:
		Server();
		~Server();

		int		createServer ();
		void	setServerAddress ( int port );
		int		bindSocket () const;
		int		listenSocket () const;

		// Getters
		int					getServerFd () const;
		struct sockaddr_in	getServerAddress () const;
};

#endif