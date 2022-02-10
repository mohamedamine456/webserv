#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <fstream>

int main (int argc, char **argv) {
	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " [PORT]" << std::endl;
		exit(EXIT_FAILURE);
	}
	else {
		int					sockfd;			// server socket FD
		int					newSockfd;		// new connection FD
		int					recvLength;		// length received in request
		struct sockaddr_in	address;		// server configuration
		struct sockaddr_in	connAddress;	// new connection configuration
		char				buffer[1024];	// request reading buffer
		std::string			headers;
		std::ofstream		file("requestbody.txt", std::ifstream::out);

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

			int bodyFound = 0;
			recvLength = recv(newSockfd, &buffer, 1024, 0);
			buffer[recvLength] = '\0';
			while (recvLength == 1024) {
				// if (std::string(buffer).find("\r\n") == std::string::npos && bodyFound < 2) {
				// 	headers.insert(0, std::string(buffer));
				// }
				// else if (std::string(buffer).find("\r\n") != std::string::npos && bodyFound <= 2) {
				// 	headers.insert(0, std::string(buffer).substr(0, std::string(buffer).find("\r\n")));
				// 	file << std::string(buffer).substr(std::string(buffer).find("\r\n"));
				// 	bodyFound++;
				// }
				// else
				// 	file << buffer;
				// recvLength = recv(newSockfd, &buffer, 1024, 0);
				// buffer[recvLength] = '\0';
			}
			buffer[recvLength] = '\0';
			if (std::string(buffer).find("\r\n") == std::string::npos && bodyFound == false) {
				headers.insert(0, std::string(buffer));
			}
			else if (std::string(buffer).find("\r\n") != std::string::npos && bodyFound == false) {
				headers.insert(0, std::string(buffer).substr(0, std::string(buffer).find("\r\n")));
				file << std::string(buffer).substr(std::string(buffer).find("\r\n"));
				bodyFound = true;
			}
			else
				file << buffer;
			file << std::endl;
			std::cout << "End Reading" << std::endl;
			std::cout << headers << std::endl;
			headers.clear();
			close(newSockfd);
		}
	}
}