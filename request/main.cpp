#include "Request.hpp"
#include "Utils.hpp"
#include "../servers/Socket.hpp"

void	read_request(int &newSockfd) {
	RequestParse	parser;
	int				recvLength;

	while ((recvLength = recv(newSockfd, &parser.buffer, RECV_SIZE, 0)) >= 0) {
		parser.buffer[recvLength] = '\0';
		if (recvLength > 0) {
			add_buffer(parser, recvLength);
			if (parser.rlf == false)
				check_requestLine(parser);
			if (parser.hf == false)
				check_headers(parser);
			if (parser.hf == true && parser.rlf == true)
				break ;
		}
	}
	// if (parser.headers.find("Content-Length:") != std::string::npos)
	// {
	// 	read_content_length(parser, newSockfd);
	// }
	// else if (parser.headers.find("Transfer-Encoding:") != std::string::npos){
	// 	read_chunked(parser, newSockfd);
	// }
	// else {
	// 	std::cout << "Nothing" << std::endl;
	// }
	std::cout << "Request Line: " << parser.requestLine << std::endl;
	std::cout << "Headers: " << parser.headers << std::endl;
	// remove(parser.filename.c_str());
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
			fcntl(newSockfd, F_SETFL, O_NONBLOCK);
			if (newSockfd < 0) {
				std::cerr << "Accepting Connection Failed!" << std::endl;
				exit(EXIT_FAILURE);
			}
			handle_request(newSockfd);
		}
	}
}