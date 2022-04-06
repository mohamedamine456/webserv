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
#include <poll.h>
#include "../srcs/request/Request.hpp"
#include "../servers/SocketInfos.hpp"

#define MAX_SERVERS 1
#define PORT 8000

void	read_request(int &newSockfd) {
	int					recvLength = 1024;								// length received in request
	char				buffer[1024];									// request reading buffer
	std::string			filename = "/var/tmp/request_" + randomfilename("");
	std::ofstream		rqstFile(filename, std::ofstream::out);
	std::cout << "Receiving:" << std::endl;
	while ((recvLength = recv(newSockfd, &buffer, 4096, 0)) == 4096) {
		buffer[recvLength] = '\0';
		rqstFile << buffer;
	}
	buffer[recvLength] = '\0';
	rqstFile << buffer;
	// remove(filename.c_str());
}

void	send_simple_response(int &newSockfd)
{
	std::string str_send = "HTTP/1.1 200 OK\nServer: Test Server\nContent-Type: text/plain\nContent-Length: 7\n\nHello!\n";
	send(newSockfd, str_send.c_str(), strlen(str_send.c_str()), 0);
}

std::vector<SocketInfos>		create_multiple_servers()
{
	std::vector<SocketInfos>		servers;
	for (int i = 0; i < MAX_SERVERS; i++)
	{
		SocketInfos	serv;
		
		// creation of socket
		if (serv.createSocket() < 0) {
			perror("Socket");
			exit(EXIT_FAILURE);
		}

		// initialize address
		serv.setSocketAddress(PORT + (i * 100));

		if (serv.bindSocket() == -1) {
			perror("Bind");
			exit(EXIT_FAILURE);
		}

		// listen on socket
		if (serv.listenSocket() == -1) {
			perror("Listen");
			exit(EXIT_FAILURE);
		}
		servers.push_back(serv);
	}
	return servers;
}

void	handle_request(int newSockfd)
{
	read_request(newSockfd);						// read request
	send_simple_response(newSockfd);							// to prevent multi request from mozilla
	std::cout << "End Reading!" << std::endl;
	close(newSockfd);
}

int main() {

    std::vector<SocketInfos>	servers = create_multiple_servers();
	int					sockfd;											// server socket FD
	int					newSockfd;										// new connection FD										// server configuration
	struct sockaddr_in	connAddress;
	socklen_t stor_size = sizeof(struct sockaddr_in);


    struct pollfd pfds[servers.size() + 1];
    int i = 0;
    for (std::vector<SocketInfos>::iterator it = servers.begin(); it != servers.end(); it++, i++) {
		pfds[i].fd = it->getSocketFd();
        pfds[i].events = POLL_IN;
	}

    while (true) {
        int num_events = poll(pfds, servers.size(), -1);
        if (num_events == -1) {
            std::cerr << "Poll Failed\n";
            exit(EXIT_FAILURE);
        }
        else if (num_events == 0) {
            std::cerr << "Poll Timed out\n";
            exit(EXIT_SUCCESS);
        }
        else {
            for (i = 0; i < servers.size(); i++) {
                if (pfds[i].revents & POLL_IN) {
                    handle_request(servers[i].getSocketFd());
                }
            }
        }
    }
}