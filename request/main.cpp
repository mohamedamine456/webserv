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

std::string getfilename() {
	static int a = 1;
	time_t ttime = std::time(0);
	std::string filename(std::to_string(ttime));
	filename.insert(filename.length(), std::to_string(a));
	a++;
	return (filename);
}

struct RequestParse {
	std::string		filename;
	char			buffer[RECV_SIZE];
	std::string		requestLine;
	bool			rlf;
	std::string		headers;
	bool			hf;
	std::ofstream	rqstFile;

	RequestParse() {
		filename = "/var/tmp/request_" + getfilename();
		rqstFile.open(filename, std::ofstream::out);
		rlf = false;
		hf = false;
	}
};

void	add_buffer( RequestParse &parser ) {
	if (parser.rlf == false) {
		parser.requestLine += std::string(parser.buffer);
	}
	else if (parser.hf == false) {
		parser.headers += std::string(parser.buffer);
	}
	else {
		parser.rqstFile << parser.buffer;
	}
}

void	check_requestLine( RequestParse &parser ) {
	if (parser.rlf == false) {
		if (parser.requestLine.find("\r\n") != std::string::npos)
		{
			parser.headers = parser.requestLine.substr(parser.requestLine.find("\r\n") + 2);
			parser.requestLine = parser.requestLine.substr(0, parser.requestLine.find("\r\n"));
			parser.rlf = true;
		}
	}
}

void	check_headers( RequestParse &parser ) {
	if (parser.hf == false) {
		if (parser.headers.find("\r\n\r\n") != std::string::npos)
		{
			std::string rest = parser.headers.substr(parser.headers.find("\r\n\r\n") + 4);
			parser.headers = parser.headers.substr(0, parser.headers.find("\r\n\r\n"));
			parser.rqstFile << rest;
			parser.hf = true;
		}
	}
}

void	read_request(int newSockfd) {
	RequestParse	parser;
	int				recvLength;

	while ((recvLength = recv(newSockfd, &parser.buffer, RECV_SIZE, 0))) {
		parser.buffer[recvLength] = '\0';
		add_buffer(parser);
		if (parser.rlf == false) {
			check_requestLine(parser);
		}
		if (parser.hf == false) {
			check_headers(parser);
		}
		break;
	}
	std::cout << "Request Line: " << parser.requestLine << std::endl << std::endl;
	std::cout << "Headers: " << parser.headers << std::endl;
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