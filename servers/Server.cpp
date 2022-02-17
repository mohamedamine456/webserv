#include "Server.hpp"

Server::Server() {
	this->serverFd = -1;
	memset((char *)&(this->serverAddress), 0, sizeof(this->serverAddress));
}

Server::~Server() {

}

int		Server::createServer () {
	this->serverFd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->serverFd == -1) {
		return (-1);
	}
	if(fcntl(this->serverFd, F_SETFL, O_NONBLOCK) < 0) {
		return (-1);
	}
	return (0);
}

void	Server::setServerAddress ( int port ) {
	this->serverAddress.sin_family = AF_UNSPEC;
	this->serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	this->serverAddress.sin_port = htons(port);
}

int		Server::bindSocket () const {
	if (bind(this->serverFd, (struct sockaddr *)&(this->serverAddress), sizeof(this->serverAddress)) == -1) {
		return (-1);
	}
	return (0);
}

int		Server::listenSocket () const {
	if (listen(this->serverFd, 100) < 0) {
		return (-1);
	}
	return (0);
}

int					Server::getServerFd () const {
	return (this->serverFd);
}

struct sockaddr_in	Server::getServerAddress () const {
	return (this->serverAddress);
}