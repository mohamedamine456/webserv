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

std::string getfilename() {
	static int a = 1;
	time_t ttime = std::time(0);
	std::string filename(std::to_string(ttime));
	filename.insert(filename.length(), std::to_string(a));
	a++;
	return (filename);
}

int main (int argc, char **argv) {
	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " [PORT]" << std::endl;
		exit(EXIT_FAILURE);
	}
	else {
		int					sockfd;											// server socket FD
		int					newSockfd;										// new connection FD
		int					recvLength = 1024;								// length received in request
		struct sockaddr_in	address;										// server configuration
		struct sockaddr_in	connAddress;									// new connection configuration
		char				buffer[1024];									// request reading buffer
		// creation of socket
		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (sockfd <= 0) {
			std::cerr << "Socket Creation Failed!" << std::endl;
			exit(EXIT_FAILURE);
		}

		// fill address with zeros
		memset((char *)&address, 0, sizeof(address));
		address.sin_family = AF_UNSPEC;
		address.sin_addr.s_addr = htonl(INADDR_ANY);
		address.sin_port = htons(std::stoi(argv[1]));

		if (bind(sockfd, (struct sockaddr *)&address, sizeof(address)) == -1) {
			std::cerr << "Bind Failed!" << std::endl;
			exit(EXIT_FAILURE);
		}

		if (listen(sockfd, 5) < 0) {
			std::cerr << "Listen Failed!" << std::endl;
			exit(EXIT_FAILURE);
		}

		while (true) {
			std::cout << "Waiting for connection on port " << argv[1] << "..." << std::endl;

			socklen_t stor_size = sizeof(struct sockaddr_in);
			newSockfd = accept(sockfd, (struct sockaddr *)&connAddress, &stor_size);

			if (newSockfd < 0) {
				std::cerr << "Accepting Connection Failed!" << std::endl;
				exit(EXIT_FAILURE);
			}
			std::string			filename = "/var/tmp/" + getfilename();
			std::ofstream		file(filename, std::ifstream::out);
			std::cout << "Receiving:" << std::endl;
			while ((recvLength = recv(newSockfd, &buffer, 1024, 0)) == 1024) {
				buffer[recvLength] = '\0';
				file << buffer;
			}
			buffer[recvLength] = '\0';
			file << buffer;
			file << std::endl;
			sleep(3);
			std::string str_send = "HTTP/1.1 200 OK\nServer: Test Server\nContent-Type: text/plain\nContent-Length: 7\n\nHello!\n";
			send(newSockfd, str_send.c_str(), strlen(str_send.c_str()), 0);
			// remove(filename.c_str());
			std::cout << "End Reading!" << std::endl;
			close(newSockfd);
		}
	}
}