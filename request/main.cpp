#include "RequestParser.hpp"
#include "Utils.hpp"
#include "../servers/Socket.hpp"

void	read_request(int &newSockfd) {
	RequestParser	parser;
	int				recvLength;

	while ((recvLength = recv(newSockfd, &parser.buffer, RECV_SIZE, MSG_DONTWAIT)) > 0) {
		parser.buffer[recvLength] = '\0';
		parser.add_buffer(recvLength);
		if (parser.getLineSet() == false)
			parser.check_requestLine();
		if (parser.getHeadersSet() == false)
			parser.check_headers();
		if (parser.getHeadersSet() == true && parser.getLineSet() == true)
			break ;
	}
	if (parser.getHeaders().find("Content-Length:") != std::string::npos)
	{
		parser.read_content_length(newSockfd);
	}
	else if (parser.getHeaders().find("Transfer-Encoding:") != std::string::npos){
		parser.read_chunked(newSockfd);
	}
	else {
		std::cout << "Nothing" << std::endl;
	}
	if (parser.getRequestLine() != "" && parser.getHeaders() != "") {
		std::cout << "Request Line: " << parser.getRequestLine() << std::endl;
		std::cout << "Headers:\n" << parser.getHeaders() << std::endl;
	}
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
	// std::cout << "End Reading!" << std::endl;
	close(newSockfd);
}

int main() {
	std::vector<Socket>	servers = create_multiple_servers();										// server socket FD
	struct sockaddr_in	connAddress;
	socklen_t stor_size = sizeof(struct sockaddr_in);
	std::vector<int>	clients;

	// start
	fd_set	rfds, rset;
	int maxfd = -1;
	unsigned int i, status;
	FD_ZERO(&rfds);
	for (std::vector<Socket>::iterator it = servers.begin(); it != servers.end(); it++) {
		FD_SET((*it).getSocketFd(), &rfds);
		if ((*it).getSocketFd() > maxfd)
			maxfd = (*it).getSocketFd();
	}
	
	while (true) {
		rset = rfds;
		for (std::vector<int>::iterator it = clients.begin(); it != clients.end(); it++) {
			FD_SET((*it), &rset);
			if ((*it) > maxfd)
				maxfd = (*it);
		}
		status = select(maxfd + 1, &rset, NULL, NULL, NULL);
		if (status < 0) {
			std::cerr << "Select Failed!" << std::endl;
			exit(EXIT_FAILURE);
		}
		for (std::vector<Socket>::iterator it = servers.begin(); it != servers.end(); it++) {
			if (FD_ISSET((*it).getSocketFd(), &rset)) {
				int newSockfd = accept((*it).getSocketFd(), (struct sockaddr *)&connAddress, &stor_size);
				if (newSockfd < 0) {
					std::cerr << "Accepting Connection Failed!" << std::endl;
					exit(EXIT_FAILURE);
				}
				fcntl(newSockfd, F_SETFL, O_NONBLOCK);
				clients.push_back(newSockfd);
				// break ;
			}
		}
		for(std::vector<int>::iterator it = clients.begin(); it != clients.end(); it++) {
			if (FD_ISSET((*it), &rset)) {
				handle_request((*it));
				std::vector<int>::iterator tmpIt = it - 1;
				close((*it));
				clients.erase(it);
				it = tmpIt;
				// break ;
			}
		}
	}
}