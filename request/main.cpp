#include "Request.hpp"
#include "Utils.hpp"
#include "../servers/Socket.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <cstdio>
#include <fcntl.h>
#define RECV_SIZE 4096

struct RequestParse {
	std::string	requestLine;
	std::string	headers;
};

void	read_request(int newSockfd) {


}

void	send_simple_response(int &newSockfd)
{
	std::string str_send = "HTTP/1.1 200 OK\nServer: Test Server\nContent-Type: text/plain\nContent-Length: 7\n\nHello!\n";
	send(newSockfd, str_send.c_str(), strlen(str_send.c_str()), 0);
}

void	handle_request(int newSockfd)
{
	read_request(newSockfd);									// read request
	send_simple_response(newSockfd);							// to prevent multi request from mozilla
	std::cout << "End Reading!" << std::endl;
	close(newSockfd);
}

int main() {
	std::vector<Socket>	servers = create_multiple_servers();
	int					sockfd;											// server socket FD
	int					newSockfd;										// new connection FD										// server configuration
	struct sockaddr_in	connAddress;
	socklen_t stor_size = sizeof(struct sockaddr_in);

	fd_set	rfds, rset;
	int maxfd = -1, fd = -1;
	unsigned int i, status;
	FD_ZERO(&rfds);
	for (std::vector<Socket>::iterator it = servers.begin(); it != servers.end(); it++) {
		FD_SET((*it).getSocketFd(), &rfds);
		if ((*it).getSocketFd() > maxfd)
			maxfd = (*it).getSocketFd();
	}
	while (true) {
		rset = rfds;
		status = select(maxfd + 1, &rset, NULL, NULL, NULL);
		if (status < 0) {
			std::cerr << "Select Failed!" << std::endl;
			exit(EXIT_FAILURE);
		}
		for (i = 0; i < servers.size(); i++) {
			if (FD_ISSET(servers[i].getSocketFd(), &rset)) {
				fd = servers[i].getSocketFd();
				break ;
			}
		}
		if (fd == -1) {
			std::cerr << "No Connection Failed!" << std::endl;
			exit(EXIT_FAILURE);
		}
		else {
			newSockfd = accept(fd, (struct sockaddr *)&connAddress, &stor_size);
			if (newSockfd < 0) {
				std::cerr << "Accepting Connection Failed!" << std::endl;
				exit(EXIT_FAILURE);
			}
			handle_request(newSockfd);
		}
	}
}