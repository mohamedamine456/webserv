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
#include "request/Request.hpp"

#define MAX_SERVERS 5

struct  server {
	int					sockfd;											// server socket FD
	int					newSockfd;										// new connection FD
	struct sockaddr_in	address;										// server configuration
	struct sockaddr_in	connAddress;									// new connection configuration
};

std::string getfilename(std::string str) {
	static int a = 1;
	time_t ttime = std::time(0);
	std::string filename(std::to_string(ttime));
	filename.insert(filename.length(), std::to_string(a));
	a++;
	return (filename);
}

void	initialize_address(struct sockaddr_in &address, int	port) {
	memset((char *)&address, 0, sizeof(address));
	address.sin_family = AF_UNSPEC;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = htons(port);
}

Request	read_request(int &newSockfd) {
	Request				rqst;
	int					recvLength = 1024;								// length received in request
	char				buffer[1024];									// request reading buffer
	rqst.setRequestfile("/var/tmp/request_" + getfilename(""));
	std::ofstream		rqstFile(rqst.getRequestfile());
	// std::ofstream		requestFile(requestFilename, std::ofstream::out);
	std::cout << "Receiving:" << std::endl;
	while ((recvLength = recv(newSockfd, &buffer, 1024, 0)) == 1024) {
		buffer[recvLength] = '\0';
		rqstFile << buffer;
	}
	buffer[recvLength] = '\0';
	rqstFile << buffer;
	std::cout << rqst.getRequestfile() << "\n";
	return rqst;
}

server	setup_server() {
	static int	port = 8080;
	server	tmp;
	tmp.sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (tmp.sockfd <= 0) {
		std::cerr << "Socket Creation Failed!" << std::endl;
		exit(EXIT_FAILURE);
	}

	initialize_address(tmp.address, port);
	port += 10;
	return tmp;
}

void	send_simple_response(int &newSockfd)
{
	std::string str_send = "HTTP/1.1 200 OK\nServer: Test Server\nContent-Type: text/plain\nContent-Length: 7\n\nHello!\n";
	send(newSockfd, str_send.c_str(), strlen(str_send.c_str()), 0);
}

int main () {

	int					sockfd;											// server socket FD
	int					newSockfd;										// new connection FD
	struct sockaddr_in	address;										// server configuration
	struct sockaddr_in	connAddress;
	
	// creation of socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd <= 0) {
		std::cerr << "Socket Creation Failed!" << std::endl;
		exit(EXIT_FAILURE);
	}

	// initialize address
	initialize_address(address, 8080);

	if (bind(sockfd, (struct sockaddr *)&address, sizeof(address)) == -1) {
		std::cerr << "Bind Failed!" << std::endl;
		exit(EXIT_FAILURE);
	}

	// listen on socket
	if (listen(sockfd, 5) < 0) {
		std::cerr << "Listen Failed!" << std::endl;
		exit(EXIT_FAILURE);
	}

	fd_set	current, backup;
	FD_ZERO(&current);
	FD_SET(sockfd, &current);

	while (true) {
		if (select(FD_SETSIZE, &current, NULL, NULL, NULL) < 0) {
			std::cerr << "Select Failed!" << std::endl;
			exit(EXIT_FAILURE);
		}

		for (int i = 0; i < FD_SETSIZE; i++) {
			if (FD_ISSET(i, &current)) {
				if (i == sockfd) {
					socklen_t stor_size = sizeof(struct sockaddr_in);
					int newSockfd = accept(sockfd, (struct sockaddr *)&connAddress, &stor_size);
					if (newSockfd < 0) {
						std::cerr << "Accepting Connection Failed!" << std::endl;
						exit(EXIT_FAILURE);
					}
					FD_SET(newSockfd, &current);
				}
				else {
					Request rqst = read_request(i);
					send_simple_response(i);
					FD_CLR(i, &current);
				}
			}
		}
	}
}